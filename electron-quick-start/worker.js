const { add, getAllClientes } = require('./build/Release/cppserver.node');

var result = getAllClientes(); 
postMessage(result);