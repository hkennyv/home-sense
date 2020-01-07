# home-sense
author(s): khuynh

## overview
This repository contains the code for my home-sense project. I have some extra
ESP32 dev boards laying around and I'd like to start interfacing with some
sensors I have lying around. Ideally, I can trial some of the sensors I'd like
to make into my [kwatch](https://github.com/hkennyv/kwatch) project.

I'd like to extend this so that I can have multiple devices in my house all
connected to the same backend and a single front-end to control/view the data.

## repository structure

## architecture
Off the top of my head, I'd like to have the ESP32 dev boards communicate to a
backend via HTTP and maybe post analog senses every 1-10 minutes to an API that
will insert into a pg database (possibly install the timescaledb extension,
have to weigh the pros and cons still...). I will most likely use a DB i aleady
have being hosted on one of my VPS. Then for the frontend, I'd like to deploy a
react app that consumes the backend API to netlify.
