curl -s --data @../vault/pathology-credentials.json http://127.0.0.1:8200/v1/auth/approle/login | json_pp | grep client_token | awk '{print $3}' | sed 's/"\|,\|\/r//g' | grep .
