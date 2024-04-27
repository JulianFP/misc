#! /usr/bin/env nix-shell
#! nix-shell -i bash --packages ffmpeg alass
#
# english subtitles only
# $1: directory relative to mediata
# $2: filename without ending
#
set -euxo pipefail #exit on any kind of error
scp "root@192.168.3.102:/mnt/mediadata/$1/$2.mkv" .
scp "root@192.168.3.102:/mnt/mediadata/$1/$2.eng.srt" .
alass-cli -g "$2.mkv" "$2.eng.srt" output.srt
ffmpeg -y -loglevel "repeat+info" -i "$2.mkv" -i "output.srt" -c copy -map 0 -dn -map "-0:s" -map "-0:d" -map "1:0" "-metadata:s:s:0" "language=eng" "-metadata:s:s:0" "handler_name=English" "-metadata:s:s:0" "title=English" "out.mkv"
ssh "root@192.168.3.102" "rm \"/mnt/mediadata/$1/$2.mkv\""
ssh "root@192.168.3.102" "rm \"/mnt/mediadata/$1/$2.eng.srt\""
scp out.mkv "root@192.168.3.102:/mnt/mediadata/$1/$2.mkv"
ssh "root@192.168.3.102" "chown -R jellyfin:jellyfin \"/mnt/mediadata/$1\""
