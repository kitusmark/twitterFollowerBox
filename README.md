twitterFollowerBox
==================
Arduino + Ethernet shield + 7segment displays = **Twitter Followers Box**


This is just a simple project where I use some IoT geeky tech. The Arduino is connected to the internet and grabs some information (Twitter Followers from a public account) and simply displays it in some cool 7-segment displays.

How to get your data
=================
Since Twitter shut down the API1.0, it's a little bit more difficult to get up and running. Basically you need to set up an 'App' in the [twitter developers section](dev.twitter.com) and make a http request using the GET method.

Since this is a pain in the ass for Arduino to acomplish. I've set a simple *php* script in my server that gets the number for me and the arduino gets it from the page served. It's kind of messy but we'll get along.


Resources
=========
This project uses the awesome library from [J7mbo](https://github.com/J7mbo/twitter-api-php), a *PHP* Wrapper for Twitter API 1.1

You can get all your questions answered by going to this [post](http://stackoverflow.com/questions/12916539/simplest-php-example-for-retrieving-user-timeline-with-twitter-api-version-1-1/15314662#15314662).
