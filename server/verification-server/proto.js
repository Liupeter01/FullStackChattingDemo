const path = require('path');
const grpc = require('@grpc/grpc-js');
const proto_loader = require('@grpc/proto-loader');

/*go and find proto/message.proto file*/
const proto_package_loaded = proto_loader.loadSync('../proto/message.proto', 
{
    keepCase: true, //disable camel case
    longs: String,  //long will be converted to string, avoiding overflow issue
    enums: String,  //enum will be converted to string
    defaults: true, //
    oneofs: true    //protocol buffers' oneof traits
});

let message_proto = grpc.loadPackageDefinition(proto_package_loaded).message;

module.exports = message_proto;