#include "WifiConnector.h"


void setup() {
     Serial.begin(9600);

    WifiConnector::Params wifi_connector_params;
    wifi_connector_params.cridentals_list_file_path = "/config_files/wifi_credentals.json";
    WifiConnector wifi_connector(wifi_connector_params);
    wifi_connector.connect_to_wifi();
}

void loop() {

  // put your main code here, to run repeatedly:

}
