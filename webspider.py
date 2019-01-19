import requests
import select
from bs4 import BeautifulSoup
from urllib.parse import urljoin
import re

def download(url,user_agent='ws',number=2,charset='utf-8',proxies=None):
    print('Downloading:',url)
    #request.add_header('User-agent',user_agent)
    headers = {'User-agent': user_agent}
    try:
        resp = requests.get(url,headers=headers,proxies=proxies)
        html = resp.text
        if resp.status_code >= 400:
            print('Download:',resp.text)
            html = None
            if number and 500 <= resp.status_code <600:
                return download(url,number-1)
    except requests.exceptions.RequestException as e:
        #print('Download:',e.reason)
        html = None
    return html


def ws_sitemap(starturl):
    sp_queue = [starturl]
    beurl = 'https://movie.douban.com/subject/'
    curl = sp_queue.pop()
    chtml = download(curl)
    get_mvname(chtml)
    for link in get_links(chtml):
        alink = urljoin(beurl,link)
        sp_queue.append(alink)
    #print(sp_queue)
    seen = set(sp_queue)
    while len(sp_queue) < 100:
        url = sp_queue.pop()
    
        html = download(url)
        get_mvname(html)
        if not html:
            return None
        for link in get_links(html):
            alink = urljoin(beurl,link)
            if alink not in seen:
                seen.add(alink)
                sp_queue.append(alink)
        #print(len(sp_queue),sp_queue)

def get_links(html):
    web_re = re.compile("""<a[^>]+href=["']https://movie.douban.com/subject/(.*?)/?from=subject-page["']""",re.IGNORECASE)
    return web_re.findall(html)
   
def get_mvname(html,):
    
    soup = BeautifulSoup(html,'html.parser')
    tag = soup.select('a[data-name]')

    for i in range(len(tag)):
        tag[i].encode('utf-8')
        #print(tag[i])
        soup1 = BeautifulSoup(str(tag[i]),'html.parser')
        print(soup1.a.get('data-name'),soup1.a.get('data-desc'))
        #str1  = soup1.a.get('data-name') + soup1.a.get('data-desc')
        
ws_sitemap('https://movie.douban.com/subject/27002092/')

