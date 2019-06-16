import requests
import json

file = open('bitzb.txt','w')
bitchain = 'https://chain.api.btc.com/v3'
resp = requests.get(bitchain+'/block/2')

for i in range(10):
    resp = requests.get(bitchain+'/block'+str(i))
    #html = json.loads(resp.text)
    file.write(resp.text)

#print(html['data']['height'],html['data']['hash'])
file.close()
