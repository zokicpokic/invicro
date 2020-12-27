vault_token:
	$(eval VAULT_TOKEN=$(shell (curl -s --data @vault/pathology-credentials.json http://127.0.0.1:8200/v1/auth/approle/login | json_pp | grep client_token | awk '{print $3}' | sed 's/"\|,\|\/r//g' | grep .)))
	echo VAULT_TOKEN: '$(VAULT_TOKEN)'

create_token: vault_token
	$(eval TS_TOKEN=$(shell (curl -s -X POST --data 'auth_token=$(VAULT_TOKEN)' http://127.0.0.1:5001/create | grep token | sed 's/ \|"//g' | sed 's/:/=/g')))
	echo TS_TOKEN: '$(TS_TOKEN)'

fronted_url_params: create_token
	# echo '?$(TS_TOKEN)&auth_token=$(VAULT_TOKEN)'
	$(eval FRONTEND_URL_PARAMS='?$(TS_TOKEN)&auth_token=$(VAULT_TOKEN)')
	echo FRONTEND_URL_PARAMS: '$(FRONTEND_URL_PARAMS)'

validate_url: fronted_url_params
	$(eval VALIDATE_URL=http://localhost:5001/validate$(FRONTEND_URL_PARAMS))
	echo VALIDATE_URL: '$(VALIDATE_URL)'

call_validation: validate_url
	curl $(VALIDATE_URL)


