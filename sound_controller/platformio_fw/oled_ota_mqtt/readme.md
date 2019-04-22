
Do not forget to add in the folder /data where you can include:
* ca.crt
* cert.der
* private.der

You can convert the cert.crt and the private.key files to .der with:
```
 openssl x509 -in client.crt -out cert.der -outform DER
 openssl rsa -in client.key -out private.der -outform DER
```
Afterwards, you can upload the /data folder to the SPIFFS OF THE nodeMCU with: `platformio run --target uploadfs`
