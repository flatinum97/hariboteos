FROM ubuntu:18.04

RUN apt update && \
      apt -y install \
        build-essential \
        nasm \
        mtools && \
      mkdir -p /mnt/

WORKDIR /mnt
