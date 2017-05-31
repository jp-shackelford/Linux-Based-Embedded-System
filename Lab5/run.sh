./tank &
target_PID="$(pgrep -f tank)"
./sensor $target_PID
