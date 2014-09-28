<?php
//The Arduino will get this webpage and get what is between < >
//We'll get the next info:
//Twitter's Followers
//from RepRapBCN

require_once('TwitterAPIExchange.php');

/** Set access tokens here - see: https://dev.twitter.com/apps/ **/
$settings = array(
'oauth_access_token'		 => "FILL YOUR TOKEN HERE",
'oauth_access_token_secret'	 => "FILL YOUR TOKEN HERE",
'consumer_key'				 => "FILL YOUR TOKEN HERE",
'consumer_secret' 			=> "FILL YOUR TOKEN HERE"
);

$ta_url = 'https://api.twitter.com/1.1/statuses/user_timeline.json';
$getfield = '?screen_name=reprapbcn';
$requestMethod = 'GET';
$twitter = new TwitterAPIExchange($settings);
$follow_count=$twitter->setGetfield($getfield)
->buildOauth($ta_url, $requestMethod)
->performRequest();
$data = json_decode($follow_count, true);
$followers_count=$data[0]['user']['followers_count'];

//Now we write the followers in the correct format for the arduino
echo '<'. $followers_count .'>';

?>