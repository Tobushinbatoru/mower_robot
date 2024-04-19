echo "updating farmware(arduino mega2560)"
echo "connect arduino mega2560 on USB"

cd `dirname ${0}`/../training_test
platformio run

# `${0}` は現在実行中のスクリプトやコマンドのパスを表します。
# `dirname` はこのパスのディレクトリ部分を抽出します。
# `../` ディレクトリ構造で1つ上の階層に移動
