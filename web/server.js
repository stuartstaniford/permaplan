// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Back end server for talking to mobile clients.

var express = require('express');
var helmet = require('helmet')

var server = express();

server.use(helmet());

server.set('view engine', 'ejs');

server.use(express.static(__dirname + '/views'));

//server.engine('html', require('ejs').renderFile);

server.get('/',               (req, res) => {res.render('index'); });

var port = 8080;
server.listen(port, function() {
    console.log('server listening on port ' + port);
});
