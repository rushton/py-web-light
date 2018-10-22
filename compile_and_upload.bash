arduino-cli compile --fqbn arduino:avr:uno web-light && \
arduino-cli upload -p $(arduino-cli board list | awk -F'\t' 'NR==2{print $2}') --fqbn arduino:avr:uno web-light && \
web-light/*.hex web-light/*.elf
