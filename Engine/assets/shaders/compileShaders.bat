rem This batch file compiles all shaders in this folder using the validator
forfiles /s /m *.vert /c "cmd /c c:/VulkanSDK/1.2.154.1/Bin/glslangValidator.exe -V @path -o @fname.vert.spv"
forfiles /s /m *.frag /c "cmd /c c:/VulkanSDK/1.2.154.1/Bin/glslangValidator.exe -V @path -o @fname.frag.spv"
pause