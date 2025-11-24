FROM ubuntu:22.04

# install requirements
RUN apt-get update && \
    apt-get install -y \
    curl \
    python3 \
    iat \
    p7zip-full \
    xdelta3 \
    wine \
    libc-bin

# extra binaries
COPY src/buildcd/cd-replace /usr/local/bin/
COPY src/buildcd/sfk /usr/local/bin/

WORKDIR /app

COPY src /app/src
COPY script /app/script
COPY signs /app/signs

WORKDIR /app/src/buildcd/
RUN rm *.eng

# Ensure the main script has execute permission
RUN chmod +x _build.sh

# Set DISPLAY to an empty value to prevent Wine from trying to connect to a display server
ENV DISPLAY=
ENV WINEDEBUG=-all

CMD ["/bin/bash", "./_build.sh"]
