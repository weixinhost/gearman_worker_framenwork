<?php

$client = new GearmanClient();
$client->addServer("127.0.0.1",4730);

$data = msgpack_pack(array(
    'a' => 1,
    'b' => 'str'
));


$result = $client->do('test',$data);

echo $result;