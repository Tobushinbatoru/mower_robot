echo "updating farmware(arduino uno)"
echo "connect arduino uno on USB"

cd `dirname ${0}`/../distance_ws
platformio run
