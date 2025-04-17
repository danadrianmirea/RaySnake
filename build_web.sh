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
  -s FORCE_FILESYSTEM=1 \
  -s EXPORTED_FUNCTIONS="['_main']" \
  -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s ENVIRONMENT=web \
  -s MODULARIZE=1 \
  -s EXPORT_NAME="RaySnake" \
  -s EXPORT_ES6=1 \
  -s USE_ES6_IMPORT_META=0 \
  -s SINGLE_FILE=1 \
  -s WASM=1 \
  -O3 \
  --preload-file Font@/Font \
  --preload-file Graphics/food.png@/Graphics/food.png \
  --preload-file Sounds@/Sounds \
  --shell-file custom_shell.html

# Check if the emcc build was successful
if [ $? -eq 0 ]; then
  echo "Build succeeded, creating web-build.zip..."
  powershell -Command "Compress-Archive -Path web-build\* -DestinationPath web-build.zip -Force"
else
  echo "Build failed, not creating zip."
fi
