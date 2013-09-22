var express = require('express');

app = express();

app.use(express.static(__dirname)); //serve static files from here.

app.listen(8888);