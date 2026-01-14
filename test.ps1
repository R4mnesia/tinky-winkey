sc.exe queryex type=service state=all | Select-String "SERVICE_NAME: tinky" -Context 0,3

tasklist | Select-String "winkey"