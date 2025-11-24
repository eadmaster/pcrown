FROM ubuntu:22.04

WORKDIR /app

COPY src /app/src
COPY script /app/script
COPY signs /app/signs

WORKDIR /app/src/buildcd/
RUN rm *.eng

# Ensure execute permissions
RUN chmod +x *.sh

# install requirements
RUN ./_build_install_deps.sh

# Set DISPLAY to an empty value to prevent Wine from trying to connect to a display server
ENV DISPLAY=
ENV WINEDEBUG=-all

CMD ["/bin/bash", "./_build.sh"]
