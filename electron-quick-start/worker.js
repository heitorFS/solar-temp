const { add } = require('./build/Release/cppserver.node');

let result = add(1, 2);
postMessage(result);