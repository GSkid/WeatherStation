FROM python:3.7-slim

ENV CONTAINER_HOME=/var/www

ADD . $CONTAINER_HOME
WORKDIR $CONTAINER_HOME

RUN sudo pip install --no-cache-dir --no-cache -r $CONTAINER_HOME/requirements.txt
COPY . .