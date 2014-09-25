<?php
//The Arduino will get this webpage and get what is between < >

//let's generate some random data and see what we got
$arduino_data = '1'.base_convert(rand(10000, 99999), 10, 36);

echo('<'.$arduino_data.'>');

?>