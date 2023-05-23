import os
import sys

node_name = sys.argv[1]
node_param_name = node_name[0].lower() + node_name[1:]
node_draw_strategy_name = node_name + "DrawStrategy"
replace_values = { 
                  "SampleNodeForAutomationParam": node_param_name, 
                  "SampleNodeDrawStrategyForAutomation": node_draw_strategy_name,
                  "SampleNodeForAutomation": node_name,
                  }

logic_sample_files = ["SampleNodeForAutomation.cpp", "SampleNodeForAutomation.hpp"]
draw_sample_files = ["SampleNodeDrawStrategyForAutomation.cpp", "SampleNodeDrawStrategyForAutomation.hpp"]
logic_file_dir = f"../LowCodeForITKApplication/Logic/Nodes/{node_name}"
draw_strategy_dir = f"../LowCodeForITKApplication/Drawing/DrawStrategies/{node_draw_strategy_name}"

def doMagic(output_dir, sample_files, replacements, append_to_file_name=""):
    for sample_file_name in sample_files:
        with open(sample_file_name, 'r') as file:
            file_extention = sample_file_name[-3:]
            file_content = file.read()
            for text_to_replace, replacement in replacements.items(): 
                file_content = file_content.replace(text_to_replace, replacement)
            
            file_name = f"{node_name+append_to_file_name}.{file_extention}"
            file_path = f"{output_dir}/{file_name}"

            if(os.path.isfile(file_path)):
                print("file exists not file creation stopped")
                exit()


            os.makedirs(output_dir, exist_ok=True)
            with open(file_path, 'w+') as node_file:
                node_file.write(file_content)


doMagic(logic_file_dir, logic_sample_files, replace_values)
doMagic(draw_strategy_dir, draw_sample_files, replace_values, append_to_file_name="DrawStrategy")
