// host = '172.16.153.122';	// hostname or IP address
host = "broker.mqttdashboard.com"; // hostname or IP address
// host = '172.16.153.110';	// hostname or IP address
port = 8000;
topic = "camera/test"; // topic to subscribe to
useTLS = false;
username = null;
password = null;
// username = "jjolie";
// password = "aa";

// path as in "scheme:[//[user:password@]host[:port]][/]path[?query][#fragment]"
//    defaults to "/mqtt"
//    may include query and fragment
//
path = "/mqtt";

// path = "/data/cloud?device=12345";

cleansession = true;

var mqtt;
var reconnectTimeout = 2000;

function MQTTconnect() {
  if (typeof path == "undefined") {
    path = "/mqtt";
  }
  mqtt = new Paho.MQTT.Client(
    host,
    port,
    path,
    "web_" + parseInt(Math.random() * 100, 10)
  );
  var options = {
    timeout: 3,
    useSSL: useTLS,
    cleanSession: cleansession,
    onSuccess: onConnect,
    onFailure: function (message) {
      $("#status").val(
        "Connection failed: " + message.errorMessage + "Retrying"
      );
      setTimeout(MQTTconnect, reconnectTimeout);
    },
  };

  mqtt.onConnectionLost = onConnectionLost;
  mqtt.onMessageArrived = onMessageArrived;

  if (username != null) {
    options.userName = username;
    options.password = password;
  }
  console.log(
    "Host=" +
      host +
      ", port=" +
      port +
      ", path=" +
      path +
      " TLS = " +
      useTLS +
      " username=" +
      username +
      " password=" +
      password
  );
  mqtt.connect(options);
}

function onConnect() {
  $("#status").val("Connected to " + host + ":" + port + path);
  // Connection succeeded; subscribe to our topic
  mqtt.subscribe(topic, { qos: 0 });
  $("#topic").val(topic);

}

function onConnectionLost(response) {
  setTimeout(MQTTconnect, reconnectTimeout);
  $("#status").val(
    "connection lost: " + responseObject.errorMessage + ". Reconnecting"
  );
}

function onMessageArrived(message) {
  var topic = message.destinationName;
  var payload = message.payloadString;

  // $('#ws').prepend('<li>' + topic + ' = ' + payload + '</li>');
  // $('#webcam').src = `data:image/jpg;base64,${payload}`
  if (payload != undefined && payload.length > 0) {
    $("#webcam").attr("src", payload);
    // console.log(payload);
  }
}

$(document).ready(function () {
  MQTTconnect();
});
