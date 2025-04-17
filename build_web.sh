. "c:\raylib\emsdk\emsdk_env.sh"
mkdir -p web-build
emcc src/*.cpp -o web-build/index.html \
  -IC:/raylib/raylib/src \
  libraylib.web.a \
  -DPLATFORM_WEB \
  -DEMSCRIPTEN_BUILD \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  -s TOTAL_MEMORY=16777216 \
  -s STACK_SIZE=262144 \
  -s FORCE_FILESYSTEM=1 \
  -s EXPORTED_FUNCTIONS="['_main']" \
  -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" \
  -s ALLOW_MEMORY_GROWTH=1 \
  --preload-file assets/Graphics/food.png@/assets/Graphics/food.png \
  --preload-file assets/Font@/assets/Font \
  --preload-file assets/Sounds@/assets/Sounds \
  --shell-file minshell.html

# Check if the emcc build was successful
if [ $? -eq 0 ]; then
  echo "Build succeeded, creating web-build.zip..."
  powershell -Command "Compress-Archive -Path web-build\* -DestinationPath web-build.zip -Force"
else
  echo "Build failed, not creating zip."
fi
