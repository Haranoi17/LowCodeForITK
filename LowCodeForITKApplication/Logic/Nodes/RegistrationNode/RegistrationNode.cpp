#include "RegistrationNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include "itkEuler2DTransform.h"
#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

#include "itkCompositeTransform.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkScaleTransform.h"
#include "itkSubtractImageFilter.h"

class CommandIterationUpdate : public itk::Command
{
  public:
    using Self       = CommandIterationUpdate;
    using Superclass = itk::Command;
    using Pointer    = itk::SmartPointer<Self>;
    itkNewMacro(Self);

  protected:
    CommandIterationUpdate() = default;

  public:
    using OptimizerType    = itk::RegularStepGradientDescentOptimizerv4<double>;
    using OptimizerPointer = const OptimizerType *;

    void Execute(itk::Object *caller, const itk::EventObject &event) override
    {
        Execute((const itk::Object *)caller, event);
    }

    void Execute(const itk::Object *object, const itk::EventObject &event) override
    {
        auto optimizer = static_cast<OptimizerPointer>(object);

        if (!itk::IterationEvent().CheckEvent(&event))
        {
            return;
        }

        std::cout << optimizer->GetCurrentIteration() << " = ";
        std::cout << optimizer->GetValue() << " : ";
        std::cout << optimizer->GetCurrentPosition() << std::endl;
    }
};

RegistrationNode::RegistrationNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    referenceImagePin = inputPins.back().get();
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    registeringImagePin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this, "lr"));
    learningRatePin = inputPins.back().get();
    inputPins.emplace_back(std::make_unique<PercentagePin>(idProvider, this, "relax"));
    relaxationFactorPin = inputPins.back().get();
    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this, "minStep"));
    minStepLengthPin = inputPins.back().get();
    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this, "iterCount"));
    iterationCountPin = inputPins.back().get();
    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this, "scaleDiv"));
    scaleDividorPin = inputPins.back().get();
    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this, "angle"));
    anglePin = inputPins.back().get();

    outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    outputImagePin = outputPins.back().get();
}

void RegistrationNode::calculate()
{
    auto referenceImage   = std::any_cast<ImageType::Pointer>(referenceImagePin->payload);
    auto registeringImage = std::any_cast<ImageType::Pointer>(registeringImagePin->payload);

    auto learningRate     = std::any_cast<float>(learningRatePin->payload);
    auto relaxationFactor = std::any_cast<float>(relaxationFactorPin->payload);
    auto minStepLength    = std::any_cast<float>(minStepLengthPin->payload);
    auto iterationCount   = std::any_cast<float>(iterationCountPin->payload);
    auto scaleDividor     = std::any_cast<float>(scaleDividorPin->payload);
    auto angle            = std::any_cast<float>(anglePin->payload);

    constexpr unsigned int Dimension = 2;
    using PixelType                  = float;

    using FixedImageType  = itk::Image<PixelType, Dimension>;
    using MovingImageType = itk::Image<PixelType, Dimension>;

    using TransformType = itk::Euler2DTransform<double>;

    using OptimizerType    = itk::RegularStepGradientDescentOptimizerv4<double>;
    using MetricType       = itk::MeanSquaresImageToImageMetricv4<FixedImageType, MovingImageType>;
    using RegistrationType = itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType, TransformType>;

    auto metric       = MetricType::New();
    auto optimizer    = OptimizerType::New();
    auto registration = RegistrationType::New();

    registration->SetMetric(metric);
    registration->SetOptimizer(optimizer);

    auto initialTransform = TransformType::New();

    using CastFilterImageToFixed = itk::RGBToLuminanceImageFilter<ImageType, FixedImageType>;
    auto castFilter1             = CastFilterImageToFixed::New();
    castFilter1->SetInput(referenceImage);
    castFilter1->Update();

    using CastFilterImageToMoving = itk::RGBToLuminanceImageFilter<ImageType, FixedImageType>;
    auto castFilter2              = CastFilterImageToMoving::New();
    castFilter2->SetInput(registeringImage);
    castFilter2->Update();

    registration->SetFixedImage(castFilter1->GetOutput());
    registration->SetMovingImage(castFilter2->GetOutput());

    using SpacingType = FixedImageType::SpacingType;
    using OriginType  = FixedImageType::PointType;
    using RegionType  = FixedImageType::RegionType;
    using SizeType    = FixedImageType::SizeType;

    FixedImageType::Pointer fixedImage = castFilter1->GetOutput();

    const SpacingType fixedSpacing = fixedImage->GetSpacing();
    const OriginType  fixedOrigin  = fixedImage->GetOrigin();
    const RegionType  fixedRegion  = fixedImage->GetLargestPossibleRegion();
    const SizeType    fixedSize    = fixedRegion.GetSize();

    TransformType::InputPointType centerFixed;

    centerFixed[0] = fixedOrigin[0] + fixedSpacing[0] * fixedSize[0] / 2.0;
    centerFixed[1] = fixedOrigin[1] + fixedSpacing[1] * fixedSize[1] / 2.0;

    MovingImageType::Pointer movingImage = castFilter2->GetOutput();

    const SpacingType movingSpacing = movingImage->GetSpacing();
    const OriginType  movingOrigin  = movingImage->GetOrigin();
    const RegionType  movingRegion  = movingImage->GetLargestPossibleRegion();
    const SizeType    movingSize    = movingRegion.GetSize();

    TransformType::InputPointType centerMoving;

    centerMoving[0] = movingOrigin[0] + movingSpacing[0] * movingSize[0] / 2.0;
    centerMoving[1] = movingOrigin[1] + movingSpacing[1] * movingSize[1] / 2.0;

    initialTransform->SetCenter(centerFixed);
    initialTransform->SetTranslation(centerMoving - centerFixed);
    initialTransform->SetAngle(angle);

    registration->SetInitialTransform(initialTransform);

    using OptimizerScalesType = OptimizerType::ScalesType;
    OptimizerScalesType optimizerScales(initialTransform->GetNumberOfParameters());

    const double translationScale = 1.0 / scaleDividor;

    optimizerScales[0] = 1.0;
    optimizerScales[1] = translationScale;
    optimizerScales[2] = translationScale;

    optimizer->SetScales(optimizerScales);

    optimizer->SetRelaxationFactor(relaxationFactor);
    optimizer->SetLearningRate(learningRate);
    optimizer->SetMinimumStepLength(minStepLength);
    optimizer->SetNumberOfIterations(iterationCount);

    constexpr unsigned int numberOfLevels = 1;

    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize(1);
    shrinkFactorsPerLevel[0] = 1;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize(1);
    smoothingSigmasPerLevel[0] = 0;

    registration->SetNumberOfLevels(numberOfLevels);
    registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
    registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

    auto observer = CommandIterationUpdate::New();
    optimizer->AddObserver(itk::IterationEvent(), observer);

    try
    {
        registration->Update();
        std::cout << "Optimizer stop condition: " << registration->GetOptimizer()->GetStopConditionDescription() << std::endl;
    }
    catch (const itk::ExceptionObject &err)
    {
        std::cout << "ExceptionObject caught !" << std::endl;
        std::cout << err << std::endl;
        return;
    }

    const TransformType::ParametersType finalParameters = registration->GetOutput()->Get()->GetParameters();

    const double finalAngle        = finalParameters[0];
    const double finalTranslationX = finalParameters[1];
    const double finalTranslationY = finalParameters[2];

    const double rotationCenterX = registration->GetOutput()->Get()->GetCenter()[0];
    const double rotationCenterY = registration->GetOutput()->Get()->GetCenter()[1];

    const unsigned int numberOfIterations = optimizer->GetCurrentIteration();

    const double bestValue = optimizer->GetValue();

    // Print out results
    //
    const double finalAngleInDegrees = finalAngle * 180.0 / itk::Math::pi;

    std::cout << "Result = " << std::endl;
    std::cout << " Angle (radians) = " << finalAngle << std::endl;
    std::cout << " Angle (degrees) = " << finalAngleInDegrees << std::endl;
    std::cout << " Translation X   = " << finalTranslationX << std::endl;
    std::cout << " Translation Y   = " << finalTranslationY << std::endl;
    std::cout << " Fixed Center X  = " << rotationCenterX << std::endl;
    std::cout << " Fixed Center Y  = " << rotationCenterY << std::endl;
    std::cout << " Iterations      = " << numberOfIterations << std::endl;
    std::cout << " Metric value    = " << bestValue << std::endl;

    using ResampleFilterType = itk::ResampleImageFilter<MovingImageType, FixedImageType>;

    auto resample = ResampleFilterType::New();
    auto scales   = optimizer->GetScales();

    using Transform = itk::CompositeTransform<double, 2>;

    resample->SetTransform(registration->GetTransform());
    resample->SetInput(movingImage);

    resample->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
    resample->SetOutputOrigin(fixedImage->GetOrigin());
    resample->SetOutputSpacing(fixedImage->GetSpacing());
    resample->SetOutputDirection(fixedImage->GetDirection());
    resample->SetDefaultPixelValue(100);

    resample->Update();

    using CastFilterToImageType = itk::CastImageFilter<FixedImageType, ImageType>;
    auto castFilter             = CastFilterToImageType::New();
    castFilter->SetInput(resample->GetOutput());
    castFilter->Update();

    registeredImage = castFilter->GetOutput();
}

void RegistrationNode::populateOutputPins()
{
    outputImagePin->payload = registeredImage;
}

json RegistrationNode::serialize()
{
    json serializedNode = Node::serialize();
    return serializedNode;
}

void RegistrationNode::deserialize(json data)
{
    Node::deserialize(data);

    referenceImagePin   = inputPins.at(0).get();
    registeringImagePin = inputPins.at(1).get();
    learningRatePin     = inputPins.at(2).get();
    relaxationFactorPin = inputPins.at(3).get();
    minStepLengthPin    = inputPins.at(4).get();
    iterationCountPin   = inputPins.at(5).get();
    scaleDividorPin     = inputPins.at(6).get();
    anglePin            = inputPins.at(7).get();
    outputImagePin      = outputPins.back().get();
}
