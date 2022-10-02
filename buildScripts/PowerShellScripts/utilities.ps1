function Check-Lockation 
{
    $current_folder = Get-Location | Split-Path -Leaf
    if($current_folder -ne "LowCodeForITK")
    {
        throw "Wrong location location is " + $current_folder + " and I need /.../LowCodeForITK"
    }
}
