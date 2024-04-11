echo "updating farmware(arduino mega2560)"
echo "connect arduino mega2560 on USB"

cd `dirname ${0}`/../mower_robot_farmware
platformio run
