/*
getVideo - $VER: getVideo 0.32 (02 Jan 2013) © 2013 Juergen Lucas
*/
options results
SIGNAL ON BREAK_C
parse arg url action downloaddir
url=strip(strip(url),'B','"')
action=lower(strip(strip(action),'B','"'))
downloaddir=strip(strip(downloaddir),'B','"')
/* config must be loaded!!! */
read_config()
address command 'ram:'
sayit(9,'Input_Url='url)
sayit(6,'')
sayit(6,'  [getVideo] v0.32')
if(action~='save' & action~='play' & action~='saveplay') then do
	answer=requestchoice('"getVideo" "What will you do?" "Play" "Save" "Save & Play" "Cancel"')
	if(answer=0) then BREAK_C()
	if(answer=1) then action='play'
	if(answer=2) then action='save'
	if(answer=3) then action='saveplay'
end
main(url)
exit
/* main ***********************************************************************/
main:
	parse arg url
	inputurl=url
	if(pos('://',url)=0) then url='http://'url
	parse var url protocol'://'domain'/'rest
	url=lower(protocol)'://'lower(domain)'/'rest
	parse var url protocol'://'domain'/'rest
	sayit(9,'Protocol='protocol)
	sayit(9,'Domain='domain)
	sayit(9,'Rest='rest)
	if(protocol='http') then do
/*		parse var url url'#'dummy */ /* am 24.05.2012 auskommentiert wegen sport1 */
		if(~check_redirect(url,'main')) then error_parse_data()
	end
	rest=lower(rest)
	extension=0
	welcome = "    try to get the video from '"domain"' - please wait..."
	if(url='' | url='help') then usage()
/*	if(rest='') then error_input_url() */
	if(protocol='mms') then do
		sayit(6,'    enter mms direct support')
		location=url;extension='wmv';title='unknown_title'
		getvideo();exit
	end
	
	if(protocol='http' & pos('.asx',rest)~=0) then do
		sayit(6,'    enter asx direct support')
		location=parse_asx(url);extension='wmv';title='unknown_title'
		getvideo();exit
	end
if(pos('amazon.',domain)~=0) then do
	sayit(6,welcome)
	location = amazon_get_location(url)
	getvideo();exit
end
if(pos('ardmediathek.',domain)~=0 || pos('mediathek.daserste',domain)~=0) then do
	sayit(6,welcome)
	location = ard_get_location(url)
	getvideo();exit
end
if(pos('foxbusiness.',domain)~=0) then do
	sayit(6,welcome)
	location = foxnews_get_location(url)
	getvideo();exit
end
if(pos('foxnews.',domain)~=0) then do
	sayit(6,welcome)
	location = foxnews_get_location(url)
	getvideo();exit
end
if(pos('funnyordie.',domain)~=0) then do
	if(funnyordie_filetype='') then funnyordie_filetype='ask'
	sayit(6,welcome)
	location = funnyordie_get_location(url)
	getvideo();exit
end
if(pos('hausgemacht.',domain)~=0) then do
	sayit(6,welcome)
	location = hausgemacht_get_location(url)
	getvideo();exit
end
if(pos('orf.at',domain)~=0) then do
	sayit(6,welcome)
	location = orf_get_location(url)
	getvideo();exit
end
if(pos('rtl-now.',domain)~=0) then do
	sayit(6,welcome)
	location = rtl_get_location(url)
	getvideo();exit
end
if(pos('sevenload.',domain)~=0) then do
	sayit(6,welcome)
	location = sevenload_get_location(url)
	getvideo();exit
end
if(pos('sport1.',domain)~=0) then do
	sayit(6,welcome)
	location = sport1_get_location(url)
	getvideo();exit
end
if(pos('superrtlnow.',domain)~=0) then do
	sayit(6,welcome)
	location = superrtl_get_location(url)
	getvideo();exit
end
if(pos('voxnow.',domain)~=0) then do
	sayit(6,welcome)
	location = vox_get_location(url)
	getvideo();exit
end
if(pos('youtube.',domain)~=0) then do
	if(youtube_filetype='') then youtube_filetype='ask'
	sayit(6,welcome)
	location = youtube_get_location(url)
	getvideo();exit
end
if(pos('zdf.',domain)~=0 & pos('mediathek',rest)~=0) then do
	sayit(6,welcome)
	location = zdf_get_location(url)
	getvideo();exit
end
	if(extension=0) then error_input_url()
	exit
amazon_get_location:
	sayit(9,'>> amazon_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		url='';
		do while(~eof(tcp))
			line = readln(tcp)
			if(url='') then parse var line 'swfParams.xmlUrl = "'url'"'
			if(url~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
	end
	else error_network_timeout()
	sayit(9,'url='url)
	if(url='') then error_parse_data()
/* open xml file */
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		uri=''; title=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(uri='') then parse var line 'video src="'uri'"'
			if(title='') then parse var line 'title="'title'"'
			if(uri~='' & title~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
	end
	else error_network_timeout()
	sayit(9,'uri='uri)
	sayit(9,'title='title)
	if(uri='') then error_parse_data()
	else extension='flv'
	if(title='') then title='unknown_title'
	else title=unescape(translate(title,' ','+'))
	sayit(9,'<< amazon_get_location')
	return uri
ard_get_location:
	sayit(9,'>> ard_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET','getVideo'))
		ardmc=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(pos('mediaCollection',line)~=0) then ardmc=1
			if(pos('</body>',line)~=0 | ardmc=1) then break
		end
		close(tcp)
		if(ardmc=1) then uri=ard_mc_get_location(url)
		else uri=ard_old_get_location(url)
	end
	else error_network_timeout()
	sayit(9,'<< ard_get_location')
	return uri
/*****************************************************************************/
ard_mc_get_location:
	sayit(9,'>> ard_mc_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET','getVideo'))
		mp4.1='';mp4.2='';mp4.3='';fms=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(fms='') then parse var line 'flashstreamingserver'd'"'fms'"'
			if(mp4.1='' & pos('Web-S',line)~=0) then mp4.1=line
			if(mp4.2='' & pos('Web-M',line)~=0) then mp4.2=line
			if(mp4.3='' & pos('Web-L',line)~=0) then mp4.3=line
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'fms='fms)
		sayit(9,'mp4.1='mp4.1);sayit(9,'mp4.2='mp4.2);sayit(9,'mp4.3='mp4.3)
		x=0; quality.1='¹'; quality.2='²'; quality.3='³'; rcbuttons=''
		do i=1 to 3; if(mp4.i~='') then do; rcbuttons=rcbuttons||'"mp4'quality.i'" '; x=x+1; answers.x=mp4.i; end; end
		if(x=0) then error_parse_data()
		if(x>1) then do
			answer=requestchoice('"getVideo" "Please select prefered filetype and quality." 'rcbuttons '"cancel"')
			sayit(9,'answer='answer)
			if(answer=0) then BREAK_C()
			uri=answers.answer
		end
		else uri=answers.1
		
		sayit(9,'uri='uri)
		if(fms~='') then uri='"'||fms||'"'||uri
		uri.1='';uri.2='';title=''
/*		parse var uri '"'uri.1'"'d'"'uri.2'?'d'clip='title'"' */
		parse var uri '"'uri.1'"'d'"'uri.2'?'d'clip='title'&'
		uri=uri.1||'/'||uri.2
		title=translate(unescape(title),' ','+')
		sayit(9,'uri='uri)
		sayit(9,'title='title)
		rtmpdumpinit()
		rtmparg.url=uri
		extension='mp4'
	end
	else error_network_timeout()
	sayit(9,'<< ard_mc_get_location')
	return uri
/******************************************************************************/
ard_old_get_location:
	sayit(9,'>> ard_old_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET','getVideo'))
		title='';wmv.1='';wmv.2='';wmv.3='';mp4.1='';mp4.2='';mp4.3='';flv.1='';flv.2='';flv.3='';fms=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(fms='') then parse var line 'flashstreamingserver'd'"'fms'"'
			if(flv.1='') then parse var line dummy"avaible_url['flashmedia']['1']"dummy""""flv.1""""dummy
			if(flv.2='') then parse var line dummy"avaible_url['flashmedia']['2']"dummy""""flv.2""""dummy
			if(flv.3='') then parse var line dummy"avaible_url['flashmedia']['3']"dummy""""flv.3""""dummy
			if(wmv.1='') then parse var line dummy"avaible_url['microsoftmedia']['1']"dummy""""wmv.1""""dummy
			if(wmv.2='') then parse var line dummy"avaible_url['microsoftmedia']['2']"dummy""""wmv.2""""dummy
			if(wmv.3='') then parse var line dummy"avaible_url['microsoftmedia']['3']"dummy""""wmv.3""""dummy
			if(mp4.1='') then parse var line dummy"avaible_url['quicktime']['1']"dummy""""mp4.1""""dummy
			if(mp4.2='') then parse var line dummy"avaible_url['quicktime']['2']"dummy""""mp4.2""""dummy
			if(mp4.3='') then parse var line dummy"avaible_url['quicktime']['3']"dummy""""mp4.3""""dummy
			if(title='') then parse var line '<title>'title'</title>'
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'flv.1='flv.1);sayit(9,'flv.2='flv.2);sayit(9,'flv.3='flv.3)
		sayit(9,'wmv.1='wmv.1);sayit(9,'wmv.2='wmv.2);sayit(9,'wmv.3='wmv.3)
		sayit(9,'mp4.1='mp4.1);sayit(9,'mp4.2='mp4.2);sayit(9,'mp4.3='mp4.3)
		x=0; quality.1='¹'; quality.2='²'; quality.3='3'; rcbuttons=''
/*		do i=1 to 3; if(flv.i~='' & pos('rtmp',lower(flv.i))=0) then do; rcbuttons=rcbuttons||'"FLV-'quality.i'" '; x=x+1; answers.x=flv.i; end; end */
		do i=1 to 3; if(flv.i~='') then do; rcbuttons=rcbuttons||'"flv'quality.i'" '; x=x+1; answers.x=flv.i; end; end
		do i=1 to 3; if(wmv.i~='') then do; rcbuttons=rcbuttons||'"wmv'quality.i'" '; x=x+1; answers.x=wmv.i; end; end
		do i=1 to 3; if(mp4.i~='') then do; rcbuttons=rcbuttons||'"mp4'quality.i'" '; x=x+1; answers.x=mp4.i; end; end
		if(x=0) then error_parse_data()
		if(x>1) then do
			answer=requestchoice('"getVideo" "Please select prefered filetype and quality." 'rcbuttons '"cancel"')
			sayit(9,'answer='answer)
			if(answer=0) then BREAK_C()
			uri=answers.answer
		end
		else uri=answers.1
		if(fms~='') then uri=fms||uri
		sayit(9,'uri='uri)
		if(pos('.mp4',uri)~=0) then extension='mp4'
		if(pos('.flv',uri)~=0) then extension='flv'
		if(pos('.wmv',uri)~=0) then extension='wmv'
		do while(pos('.asx',uri)~=0 | pos('redirect.',uri)~=0)
			uri=parse_asx(uri)
			if(uri=0) then error_parse_data()
			extension='wmv'
		end
/*		if(pos('rtmp:',uri)~=0) then do */
/*		if(pos('rtmp:',uri)~=0 | pos('rtmpt:',uri)~=0 | pos('rtmpe:',uri)~=0) then do */
		if(substr(upper(strip(uri)),1,4)='RTMP') then do
			sayit(9,'rtmp url:'uri)
			rtmpdumpinit()
			if(pos('ardfs',uri)~=0 | pos('swr',uri)~=0) then parse var uri dummy'//'rtmparg.host'/'rtmparg.app'/'rtmparg.path'?'dummy
			else parse var uri rtmparg.url'?'dummy
			extension='flv'
		end
		sayit(9,'uri='uri)
		sayit(9,'ext='extension)
		sayit(9,'title='title)
		if(title='') then title='unknown_title'
	end
	else error_network_timeout()
	sayit(9,'<< ard_old_get_location')
	return uri
foxnews_get_location:
	sayit(9,'>> foxnews_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		url='';
		do while(~eof(tcp))
			line = readln(tcp)
			if(url='') then parse var line "videoId:'"url"'"
			if(url~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
	end
	else error_network_timeout()
	sayit(9,'url='url)
	if(url='') then error_parse_data()
/*	url='http://video.foxnews.com/v/feed/video/'||url||'.js' */
	url='http://'domain'/v/feed/video/'||url||'.js'
	sayit(9,'url='url)
/* open js file */
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		uri=''; title=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(uri='') then parse var line '"url":"'uri'"'
			if(title='') then parse var line '"title":"'title'"'
			if(uri~='' & title~='') then break
		end
		close(tcp)
	end
	else error_network_timeout()
	sayit(9,'uri='uri)
	sayit(9,'title='title)
	if(uri='') then error_parse_data()
	else extension='flv'
	if(title='') then title='unknown_title'
	sayit(9,'<< foxnews_get_location')
	return uri
funnyordie_get_location:
	sayit(9,'>> funnyordie_get_location')
	parse arg url
	parse var url dummy'videos/'uri'/'title
	
	title=translate(title,' ','-')
	sayit(9,'uri='uri)
	sayit(9,'title='title)
	if(uri='') then error_parse_data()
	else do
		if(funnyordie_filetype='ask') then do
			answer=requestchoice('"getVideo" "Please select  prefered quality." "FLV" "3GP" "MP4" "cancel"')
			if(answer=1) then funnyordie_filetype='flv'
			if(answer=2) then funnyordie_filetype='3gp'
			if(answer=3) then funnyordie_filetype='mp4'
		end
		extension=funnyordie_filetype
		if(extension='flv') then uri='http://videos0.ordienetworks.com/videos/'uri'/sd.flv'
		if(extension='3gp') then uri='http://videos0.ordienetworks.com/videos/'uri'/iphone_edge.3gp'
		if(extension='mp4') then uri='http://videos0.ordienetworks.com/videos/'uri'/iphone_wifi.mp4'
	end
	if(title='') then title='unknown_title'
	sayit(9,'<< funnyordie_get_location')
	return uri
hausgemacht_get_location:
	sayit(9,'>> hausgemacht_get_location')
	parse arg url
	lastslash = lastpos('/',url)
	if(lastslash~=0) then url=substr(url,lastslash+1,length(url)-lastslash-1)
	nodeid=''; vidid=''
	parse var url vidid','nodeid','dummy'.html'dummy1
	if(nodeid='' & vidid='') then error_parse_data()
	sayit(9,'nodeid='nodeid)
	sayit(9,'vidid ='vidid)
	
	url='http://www.hausgemacht.tv/sendVideoPlaylist.php?node_id='nodeid'&vid='vidid
	sayit(9,'url='url)
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		
		do while(~eof(tcp))
			line = readln(tcp)
			if(d2c(13)=line) then break
		end
		line=''
		do while(~eof(tcp)) /* damn readln doesnt work like expected */
			line=line||readch(tcp)
			if(pos('</playlist',line)~=0) then break
		end
		close(tcp)
		sayit(9,'line='line)
		line=betstr('<track>','</track>',line,1)
		sayit(9,'track='line)
		if(line=0) then error_parse_data()
		title=betstr('<title>','</title>',line,1)
		uri=betstr('<location>','</location>',line,1)
		if(title=0|uri=0) then error_parse_data()
		sayit(9,title)
		sayit(9,uri)
		extension='flv'
	end
	else error_network_timeout()
	sayit(9,'<< hausgemacht_get_location')
	return uri
orf_get_location:
	sayit(9,'>> orf_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		uri=''; title=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(uri='') then parse var line '<param'd'value="'uri'"'
			if(title='') then parse var line dummy'<title>'title'</title>'dummy
			if(uri~='' & title~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'uri='uri)
		sayit(9,'title='title)
		if(uri='') then error_parse_data()
		else do
			extension='wmv'
			uri='http://'domain||uri
			sayit(9,'uri='uri)
			uri=parse_asx(uri)
		end
		if(title='') then title='unknown_title'
	end
	else error_network_timeout()
	sayit(9,'<< orf_get_location')
	return uri
rtl_get_location:
	sayit(9,'>> rtl_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		uri='';player=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(uri='') then parse var line "data:'"uri"'"
			if(player='') then parse var line 'embedSWF("'player'"'
			if(uri~='' & player~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'uri='uri)
		sayit(9,'player='player)
		if(uri='') then error_parse_data()
		url=unescape(uri)
		if(pos('http://',url)=0) then url='http://'||domain||url
		sayit(9,'url='url)
		parse var url 'http://'domain'/'path
		if open(tcp, "TCP:"domain"/80", 'RW') then do
			writeln(tcp, create_http_request('GET'))
			uri='';title=''
			do while(~eof(tcp))
				line = readln(tcp)
				if(title='') then parse var line '<title>'d'['d'['title']'d'</title>'
				if(uri='') then parse var line '<filename'd'['d'['uri']'d'</filename>'
				if(uri~='' & title~='') then break
			end
			close(tcp)
		end
		else error_network_timeout()
		sayit(9,'title='title)
		sayit(9,'uri='uri)
		if(uri='') then error_parse_data()
		if(title='') then title='unknown_title'
	end
	else error_network_timeout()
	rtmpdumpinit()
	if(pos('.f4v',uri)~=0) then do
		extension='mp4'
		parse var uri dummy'://'rtmparg.host'/'rtmparg.app'/'rtmparg.path
		rtmparg.path='mp4:'||rtmparg.path
	end
	if(pos('.flv',uri)~=0) then do
		extension='flv'
		parse var uri dummy'://'rtmparg.host'/'rtmparg.app'/'rtmparg.path'.flv'
	end
	rtmparg.app='rtlnow'
	rtmparg.swfurl='http://rtl-now.rtl.de/'
	rtmparg.pageurl='http://rtl-now.rtl.de/'
	rtmparg.swfvfy=player
	sayit(9,'<< rtl_get_location')
	return uri
sevenload_get_location:
	sayit(9,'>> sevenload_get_location')
	parse arg url
	if(pos('-',url,lastpos('/',url))~=0) then id=substr(url,lastpos('/',url)+1,pos('-',url,lastpos('/',url))-lastpos('/',url)-1)
	else error_parse_data()
	sayit(9,'id='id)
	url='http://flash.sevenload.com/player?itemId='id
	sayit(9,'url='url)
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		line=readln(tcp)
		uri=''; title=''; oldline='';
		do while(~eof(tcp))
			line = readln(tcp)
			if(length(line)=1000 & length(oldline)<63535) then do
				if(oldline~='') then oldline=oldline||line
				else oldline=line
			end
			else do
				if(oldline~='') then do; line=oldline||line; oldline=''; end
				if(uri='') then parse var line dummy'<streams>'uri'</streams>'dummy
				if(title='') then parse var line dummy'<item'dummy'<title>'title'</title>'dummy
			end
			if(uri~='' & title~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'uri='uri)
		sayit(9,'title='title)
		streams=count('<stream',uri)
		sayit(9,'streams='streams)
		do i=1 to streams
			temp=betstr('<stream','</stream>',uri,i)
			parse var temp dummy'quality="'temp.i.1'"'dummy
			temp=betstr('<locations>','</locations>',temp,1)
			sayit(9,'temp='temp)
			parse var temp dummy'<location'dummy'>'temp.i.2'</location>'dummy
			sayit(9,'quality='temp.i.1)
			sayit(9,'location='temp.i.2)
			sayit(9,'extension='getextension(temp.i.2))
		end
		
		if(streams>1) then do
			rcbuttons=''
			do i=1 to streams
				rcbuttons=rcbuttons' "'temp.i.1'"'
			end
			answer=requestchoice('"getVideo" "Please select prefered quality."'rcbuttons' "cancel"')
			sayit(9,'answer='answer)
			if(answer=0) then BREAK_C()
		end
		else answer=1
		sayit(9,'answer='answer)
		uri=temp.answer.2
		if(pos('?',uri)~=0) then extension=delstr(uri,pos('?',uri))
		else extension=uri
		extension=getextension(extension)
		if(uri='') then error_parse_data()
		uri=html_chars_decode(uri)
		if(title='') then title='unknown_title'
		
		if(pos('rtmp',uri)~=0) then do
			rtmpdumpinit()
			rtmparg.url=uri
			extension='flv'
		end
	end
	else error_network_timeout()
	sayit(9,'<< sevenload_get_location')
	return uri
/* http://mediencenter.sport1.de/de/video/moto_gp/index.html#/0,0_gupw8tn7 */
/* http://medianac.nacamar.de/p/117/playManifest/entryId/0_gupw8tn7 */
sport1_get_location:
	sayit(9,'>> sport1_get_location')
	parse arg url
	if(pos('mediencenter.sport1.',url)=0) then error_parse_data()
	parse var url '0_'vid
	sayit(9,'vid='vid)
	sayit(9,'url='url)
	url='http://medianac.nacamar.de/p/117/playManifest/entryId/0_'||vid
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		uri=''
		do while(~eof(tcp))
			line = readln(tcp)
			sayit(9,line)
			if(uri='') then parse var line 'url="'uri'"'
			if(uri~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'uri='uri)
		if(uri='') then error_parse_data()
		extension='mp4'
		if(title='') then title='Sport1_unknown_title'
	end
	else error_network_timeout()
	sayit(9,'<< example_get_location')
	return uri
superrtl_get_location:
	sayit(9,'>> superrtl_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		uri='';player=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(uri='') then parse var line "data:'"uri"'"
			if(player='') then parse var line 'embedSWF("'player'"'
			if(uri~='' & player~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'uri='uri)
		sayit(9,'player='player)
		if(uri='') then error_parse_data()
		url=unescape(uri)
		if(pos('http://',url)=0) then url='http://'||domain||url
		sayit(9,'url='url)
		parse var url 'http://'domain'/'path
		if open(tcp, "TCP:"domain"/80", 'RW') then do
			writeln(tcp, create_http_request('GET'))
			uri='';title=''
			do while(~eof(tcp))
				line = readln(tcp)
				if(title='') then parse var line '<title>'d'['d'['title']'d'</title>'
				if(uri='') then parse var line '<filename'd'['d'['uri']'d'</filename>'
				if(uri~='' & title~='') then break
			end
			close(tcp)
		end
		else error_network_timeout()
		sayit(9,'title='title)
		sayit(9,'uri='uri)
		if(uri='') then error_parse_data()
		if(title='') then title='unknown_title'
	end
	else error_network_timeout()
	rtmpdumpinit()
	if(pos('.f4v',uri)~=0) then do
		extension='mp4'
		parse var uri dummy'://'rtmparg.host'/'rtmparg.app'/'rtmparg.path
		rtmparg.path='mp4:'||rtmparg.path
	end
	if(pos('.flv',uri)~=0) then do
		extension='flv'
		parse var uri dummy'://'rtmparg.host'/'rtmparg.app'/'rtmparg.path'.flv'
	end
	rtmparg.app='superrtlnow'
	rtmparg.swfurl='http://www.superrtlnow.de/'
	rtmparg.pageurl='http://www.superrtlnow.de/'
	rtmparg.swfvfy=player
	sayit(9,'<< rtl_get_location')
	return uri
vox_get_location:
	sayit(9,'>> vox_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		uri='';player=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(uri='') then parse var line "data:'"uri"'"
			if(player='') then parse var line 'embedSWF("'player'"'
			if(uri~='' & player~='') then break
			if(pos('</body>',line)~=0) then break
		end
		close(tcp)
		sayit(9,'uri='uri)
		sayit(9,'player='player)
		if(uri='') then error_parse_data()
		url=unescape(uri)
		if(pos('http://',url)=0) then url='http://'||domain||url
		sayit(9,'url='url)
		parse var url 'http://'domain'/'path
		if open(tcp, "TCP:"domain"/80", 'RW') then do
			writeln(tcp, create_http_request('GET'))
			uri='';title=''
			do while(~eof(tcp))
				line = readln(tcp)
				if(title='') then parse var line '<title>'d'['d'['title']'d'</title>'
				if(uri='') then parse var line dummy'<filename'd'['d'['uri']'d'</filename>'
				if(uri~='' & title~='') then break
			end
			close(tcp)
		end
		else error_network_timeout()
		sayit(9,'title='title)
		sayit(9,'uri='uri)
		if(uri='') then error_parse_data()
		if(title='') then title='unknown_title'
	end
	else error_network_timeout()
	rtmpdumpinit()
	if(pos('.f4v',uri)~=0) then do
		extension='mp4'
		parse var uri dummy'://'rtmparg.host'/'rtmparg.app'/'rtmparg.path
		rtmparg.path='mp4:'||rtmparg.path
	end
	if(pos('.flv',uri)~=0) then do
		extension='flv'
		parse var uri dummy'://'rtmparg.host'/'rtmparg.app'/'rtmparg.path'.flv'
	end
	rtmparg.app='voxnow'
	rtmparg.swfurl='http://www.voxnow.de/'
	rtmparg.pageurl='http://www.voxnow.de/'
	rtmparg.swfvfy=player
	sayit(9,'<< vox_get_location')
	return uri
/* WebProxy for blocked videos */
/* http://www.youtube.com/watch?v=MntbN1DdEP0 */
/* http://hidemyass.com/proxy/ */
youtube_get_location:
	sayit(9,'>> youtube_get_location')
	parse arg url
	sayit(9,'url='url)
	/* playlist support */
	if(pos('datatype=playlist',url)~=0 || pos('&p=',url)~=0 || pos('?p=',url)~=0 & pos('v=',url)=0) then do	/* VideoID has higher prio then PlaylistID */
/*	if(pos('datatype=playlist',url)~=0 || pos('&p=',url)~=0 || pos('?p=',url)~=0) then do	 */
		sayit(6,'    try to load the playlist site on youtube')
		if(pos('data=',url)~=0) then parse var url 'data='plstid'&'
		if(pos('?p=',url)~=0) then parse var url '?p='plstid'&'
		if(pos('&p=',url)~=0) then parse var url '&p='plstid'&'
		/* just link to the youtube playlist site, because the */
		/* old code is no more working and youtube api suxx   */
		address command 'openurl "http://www.youtube.com/view_play_list?p='||plstid||'" newwin'
		exit
	end
/* get video_id - start */
	vid=''
	if(pos('/swf/',url)~=0 & vid='') then do
		sayit(9,'youtube embedded support 1')
		parse var url 'video_id='vid'&'
	end
	if(pos('/v/',url)~=0 & vid='') then do
		sayit(9,'youtube embedded support 2')
		parse var url '/v/'vid'?'
	end
	if(pos('/embed/',url)~=0 & vid='') then do
		sayit(9,'youtube embedded support 3')
		parse var url '/embed/'vid'?'
	end
	if(pos('verify_age',url)~=0) then do /* age control video*/
		sayit(3,d2c(10)'    No support for age conrol videos.')
		exit
	end
	if(pos('v=',url)~=0 & vid='') then do
		sayit(9,'youtube standard')
		parse var url 'v='vid'&'
	end
	sayit(9,'vid='vid)
	if(vid='') then error_parse_data()
/* get video_id - end */
	url='http://www.youtube.com/watch?v='vid
	parse var url 'http://'domain'/'path
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		title='';oldline='';fmt='';fmtlist=''
		do while(~eof(tcp))
			line = readln(tcp)
			if(length(line)=1000 & length(oldline)<63535) then do
				if(oldline~='') then oldline=oldline||line
				else oldline=line
			end
			else do
				if(oldline~='') then do; line=oldline||line; oldline=''; end
				if(title='') then parse var line '<meta name="title" content="'title'">'
				if(fmt='') then parse var line '"url_encoded_fmt_stream_map":'d'"'fmt'"'
				if(fmtlist='') then parse var line '"fmt_list":'d'"'fmtlist'"'
			end
			if(title~='' & fmt~='' & fmtlist~='') then break
		end
		close(tcp)
		sayit(9,'raw_title='title)
		sayit(9,'raw_fmt='fmt)
		sayit(9,'raw_fmtlist='fmtlist)
/* check available quality - start */
		if(fmt='') then error_parse_data()
		vqo.0=12
		vqo.1 =17;	extension.17='3gp';	btxt.17='3GP¹'
		vqo.2 =36;	extension.36='3gp';	btxt.36='3GP²'
		vqo.3 =5;	extension.5 ='flv';	btxt.5 ='FLV¹'
		vqo.4 =34;	extension.34='flv';	btxt.34='FLV²'
		vqo.5 =35;	extension.35='flv';	btxt.35='FLV³'
		vqo.6 =18;	extension.18='mp4';	btxt.18='MP4'
		vqo.7 =22;	extension.22='mp4';	btxt.22='HD¹'
		vqo.8 =37;	extension.37='mp4';	btxt.37='HD²'
		vqo.9 =38;	extension.38='mp4';	btxt.38='4K'
		vqo.10=43;	extension.43='webm';	btxt.43='webM¹'
		vqo.11=44;	extension.44='webm';	btxt.44='webM²'
		vqo.12=45;	extension.45='webm';	btxt.45='webM³'
		x=0
		fmt=fmt||','							/* for better parsing later */
		do while(pos(',',fmt)~=0)
			x=x+1
			parse var fmt fmttemp','fmt				/* cut first entry from the other part */
			fmttemp=unescape(fmttemp)
			fmttemp='\u0026'||fmttemp||'\u0026'			/* for better parsing later */
			parse var fmttemp '\u0026itag='fmt_vq.x'\u0026'
			parse var fmttemp '\u0026url='fmt_url_temp'\u0026'
			parse var fmttemp '\u0026sig='fmt_sig_temp'\u0026'
			fmt_url.x=fmt_url_temp||'&signature='fmt_sig_temp
			sayit(9,'fmt_vq='fmt_vq.x)
			sayit(9,'fmt_url='fmt_url.x)
		end
		sayit(9,'fmt (must be empty)='fmt)
		fmt_vq.0=x
		l=0
		fmtlist=unescape(fmtlist)||','
		do while(pos(',',fmtlist)~=0)
			l=l+1
			parse var fmtlist res.l','
			sayit(9,d2c(10)'res='res.l)
			fmtlist=delstr(fmtlist,1,pos(',',fmtlist))
		end
		sayit(9,'fmtlist (must be empty)='fmtlist)
		res.0=l
		sayit(9,'youtube_filetype='youtube_filetype)
		f=0;ftc=0;rctext=''
		do o=1 to vqo.0					/* order by quality */
			do i=1 to fmt_vq.0
				if(fmt_vq.i=vqo.o) then do
					f=f+1
					vqf.f=fmt_vq.i
					urlf.f=fmt_url.i
					if(youtube_filetype=vqt) then ftc=f
				end
			end
			do l=1 to res.0
				parse var res.l vqt'\/'rst'\/'
				if(vqt=vqo.o) then do
					temp=vqo.o
					rctext=rctext'*n   'btxt.temp' = 'rst' ('temp')'
				end
			end
		end
		vqf.0=f
/* check available quality - end */
		if(ftc=0) then do
			if(youtube_filetype~='ask') then sayit(3,d2c(10)'  Your prefered filetype is not available.')
			rcbuttons=''
			do x=1 to vqf.0
				temp=vqf.x
				rcbuttons=rcbuttons||'"'btxt.temp'" '
			end
			sayit(9,'rcbuttons='rcbuttons)
			rctext=rctext'*n(resolution is not always correct)'
			sayit(9,'rctext='rctext)
			answer=requestchoice('"getVideo" "Please select prefered filetype:'rctext'" 'rcbuttons'"cancel"')
			sayit(9,'answer='answer)
			if(answer=0) then BREAK_C()
			uri=urlf.answer
			vq=vqf.answer
		end
		else do
			/* filetype selection */
			uri=urlf.ftc
			vq=vqf.ftc
		end
		extension=extension.vq
		sayit(9,'uri='uri)
		sayit(9,'extension='extension)
		if(title='') then title='unknown_title'
	end
	else error_network_timeout()
	sayit(9,'<< youtube_get_location')
	return uri
zdf_get_location:
	sayit(9,'>> zdf_get_location')
	parse arg url
	parse var url '/video/'vid'/'
	sayit(9,'vid='vid)
	if(vid='') then error_parse_data()
	url='http://www.zdf.de/ZDFmediathek/xmlservice/web/beitragsDetails?id='vid
	sayit(9,'url='url)
/*	address command 'openurl newwin "'url'"' */
	parse var url 'http://'domain'/'path
	if(~check_redirect(url,'main')) then error_parse_data()
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		line=readln(tcp)
		title='';x=0
		do while(~eof(tcp))
			line = readln(tcp)
			uri=''
			parse var line '<url>'uri'</url>'
			if(uri~='') then do
				x=x+1
				uri.x=uri
			end
			if(title='') then parse var line '<title>'title'</title>'
			if(pos('</video>',line)~=0) then break
		end
		close(tcp)
		uri.0=x
		if(uri.0=0) then error_parse_data()
		if(title='') then title='unknown_title'
		sayit(9,uri)
		sayit(9,title)
	end
	else error_network_timeout()
	url.1='';url.2='';url.3=''
	ext.1='wmv';ext.2='wmv';ext.3='wmv'
	but.1='"wmv¹" ';but.2='"wmv²" ';but.3='"wmv³" '
	url.4='';url.5='';url.6='';url.7=''
	ext.4='mp4';ext.5='mp4';ext.6='mp4';ext.7='mp4'
	but.4='"mp4¹" ';but.5='"mp4²" ';but.6='"mp4³" ';but.7='"HD" '
	do i=1 to uri.0
		if(pos('/veryhigh/',uri.i)~=0 & pos('.asx',uri.i)~=0) then	url.3=uri.i
		if(pos('/300/',uri.i)~=0 & pos('.asx',uri.i)~=0) then		url.2=uri.i
		if(pos('/56/',uri.i)~=0 & pos('.asx',uri.i)~=0) then 		url.1=uri.i
		if(pos('/hd/',uri.i)~=0 & pos('.meta',uri.i)~=0) then		url.7=uri.i
		if(pos('/veryhigh/',uri.i)~=0 & pos('.meta',uri.i)~=0) then 	url.6=uri.i
		if(pos('/300/',uri.i)~=0 & pos('.meta',uri.i)~=0) then 		url.5=uri.i
		if(pos('/56/',uri.i)~=0 & pos('.meta',uri.i)~=0) then 		url.4=uri.i
	end
	rcbuttons='';x=0
	do i=1 to 7
		if(url.i~='') then do
			x=x+1
			url.x=url.i
			ext.x=ext.i
			rcbuttons=rcbuttons||but.i
		end
	end
	sayit(9,rcbuttons)
	answer=requestchoice('"getVideo" "Please select prefered quality." 'rcbuttons'"cancel"')
	if(answer=0) then BREAK_C()
	extension=ext.answer
	uri=url.answer
	if(pos('.asx',uri)~=0) then uri=parse_asx(uri)
	if(pos('.meta',uri)~=0) then do
		uri=parse_meta(uri)
		rtmpdumpinit()
		uri=repstr(uri,'/zdf/mp4:','/zdf/zdf/mp4:')
		rtmparg.url=uri
	end
	if(uri=0) then error_parse_data()
	sayit(9,'<< zdf_get_location')
	return uri
betstr: PROCEDURE
	parse arg begstr,endstr,input,number
	if(number='') then number=1
	inputup=upper(input);begstrup=upper(begstr);endstrup=upper(endstr);begpos=0
	do i=1 to number;begpos=pos(begstrup,inputup,begpos+1);end
	if(begpos=0) then return 0
	endpos=pos(endstrup,inputup,begpos)
	if(endpos=0) then return 0
	output=substr(input,begpos+length(begstr),endpos-begpos-length(begstr))
	return output
BREAK_C:
	sayit(6,'')
	sayit(6,'  aborting...')
	exit
ceil: PROZEDURE
	parse arg pre'.'post
	if(post>0) then return pre+1
	else return pre
check_redirect:
	sayit(9,'>> check_redirect')
	parse arg input,backto
	if(upper(left(input,7))~='HTTP://') then input='http://'input
	parse var input 'http://'domain'/'path
	port=''
	if(pos(':',domain)~=0) then parse var domain domain':'port
	if(port='') then port=80
	if open(tcp, "TCP:"domain"/"port, 'RW') then do
		writeln(tcp, create_http_request('GET'))
		line = readln(tcp)
		sayit(9,'check_redirect: 'line)
		close(tcp)
		code=httpcode(line)
		sayit(9,'check_redirect-code='code)
		if(left(code,1)=2) then return 1
		if(left(code,1)=3) then do
			output=follow_redirect(input)
                		if(backto='getvideo') then do
				location=output
				getvideo()
				exit
			end
			if(backto='main') then do
				main(output)
				exit
			end
		end
		else do
			sayit(3,'')
			sayit(3,'  HTTP ERROR 'code)
		end
	end
	else error_network_timeout()
	sayit(9,'<< check_redirect')
	return 0
count: PROZEDURE
	parse arg countstr,input
	x=0
	do while pos(countstr,input)~=0
		input=delstr(input,1,pos(countstr,input)+length(countstr)-1)
		sayit(9,'count_input='input)
		x=x+1
	end
	return x
create_http_request:
	parse arg command
/*	agent='getVideo' */
	agent='unknown'
/*	agent='Mozilla/5.0' */
	CR=D2C(10);LF=D2C(13)
	request= command' /'path' HTTP/1.0'||LF||CR
	request= request||'User-Agent: 'agent||LF||CR
	request= request||'Host: 'domain||LF||CR
	request= request||'Pragma: no-cache'||LF||CR
	request= request||'Accept-Language: en, *'||LF||CR
	request= request||'Accept: text/html;level=3'||LF||CR
	request= request||'Accept: text/html;version=3.0'||LF||CR
	request= request||'Accept: *'d2c(47)'*'||LF||CR
	if(command='POST') then do
		request= request||'Content-type: application/x-www-form-urlencoded'||LF||CR
		request= request||'Content-length: '||postlength||LF||CR
		request= request||LF||CR
		request= request||LF||CR
		request= request||postrequest||LF||CR
	end
	request= request||LF /*||CR << get bad request at guba.com*/
/*	sayit(9,request) */
	return request
create_http_request_lw:
	parse arg command
/*	agent='getVideo' */
	agent='unknown'
/*	agent='Mozilla/5.0' */
	CR=D2C(10);LF=D2C(13)
	request= command' /'path' HTTP/1.0'||LF||CR
	request= request||'User-Agent: 'agent||LF||CR
	request= request||'Host: 'domain||LF||CR
	request= request||'Pragma: no-cache'||LF||CR
	request= request||'Accept-Language: en, *'||LF||CR
	request= request||'Accept: text/html;level=3'||LF||CR
	request= request||'Accept: text/html;version=3.0'||LF||CR
	request= request||'Accept: *'d2c(47)'*'||LF||CR
/*	request= request||'Connection: Keep-Alive'||LF||CR */
	request= request||LF /*||CR << get bad request at guba.com*/
	return request
error_input_url:
	pars arg url
	sayit(3,'')
	sayit(3,'  Sorry this URL is not supported.')
	sayit(3,'  You gave the following URL: "'inputurl'"')
	expected_url()
	exit
error_network_timeout:
	sayit(3,'')
	sayit(3,'  Can''t recieve the wanted file.')
	exit
error_parse_data:
	sayit(3,'')
	sayit(3,'  Sorry can''t get expected data.')
	exit
expected_url:
	sayit(3,'')
	sayit(3,'  Please read the documentation what URLs are supported.')
	exit
follow_redirect:
	sayit(9,'>> follow_redirect_get_location')
	parse arg url
	parse var url 'http://'domain'/'path
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('HEAD'))
		line=readln(tcp)
		sayit(9,line)
		if(left(httpcode(line),1)=2) then do
			close(tcp)
			return url
		end
		if(left(httpcode(line),1)~=3) then do
			close(tcp)
			return 0
		end
		uri=''
		do while(~eof(tcp))
			line=readln(tcp)
			parse var line dummy'Location: 'uri
			if(uri~='') then break
		end
		close(tcp)
		if(uri='') then error_parse_data()
		else do
			uri=strip(uri,'B',d2c(13))
/*			if(pos('http://',uri)=0) then uri='http://'domain''uri */
			if(pos('://',uri)=0) then uri='http://'domain''uri
		end
	end
	else error_network_timeout()
	sayit(9,'    follow redirection to: 'uri)
	sayit(6,'    follow redirection...')
	sayit(9,'<< follow_redirect_get_location')
	return uri
getextension: PROZEDURE
	parse arg input
	extension=substr(input,lastpos('.',input)+1,length(input)-lastpos('.',input))
	return extension
getvideo:
	sayit(9,'>> getvideo')
	sayit(9,'location='location)
	if(pos('://',location)=0) then location='http://'location
	parse var location protocol'://'domain'/'rest
	location=lower(protocol)'://'lower(domain)'/'rest
	parse var location protocol'://'pflocation
	if(protocol="http") then if(~check_redirect(location,'getvideo')) then error_parse_data()
	title=html_chars_decode(title)
	title=unescape(title)
	title=utf8_to_latin1(title)
	if(length(inputurl)>79) then inputurl=substr(inputurl,1,76)'...'
	sayit(6,'')
	sayit(6,'    Source: 'inputurl)
	sayit(6,'    Title:  'title)
	sayit(9,'    url:    'location)
	
	if(pos('save',action)~=0) then do
		file   = title_to_filename(title)
		target = downloaddir''file'.'extension
/*		target = targetcheck(strip(target),downloaddir,file,extension,protocol) */
		target = targetcheck()
		sayit(6,'    Target: 'target)
		sayit(6,'')
		sayit(6,'    start download please wait...')
		if(pos('rtmp',protocol)=0) then address command 'filenote "'target'" "'inputurl'" quiet'
		protocolcheck=0
		if(protocol="http") then do
			protocolcheck=1
			address command rep_placeholder(save_http)
		end
		if(protocol="mms") then do
			protocolcheck=1
			address command rep_placeholder(save_mms)
			/* filenote once again, because mplayer delete the filenote on SFS partition */
			address command 'filenote "'target'" "'inputurl'" quiet'
		end
		if(pos('rtmp',protocol)~=0) then do
			protocolcheck=1
			if(rtmparg.url~='') then rtmpargs=rtmpargs||'-r "'rtmparg.url'" '
			if(rtmparg.app~='') then rtmpargs=rtmpargs||'--app "'rtmparg.app'" '
			if(rtmparg.host~='') then rtmpargs=rtmpargs||'--host "'rtmparg.host'" '
			if(rtmparg.path~='') then rtmpargs=rtmpargs||'--playpath "'rtmparg.path'" '
			if(rtmparg.swfurl~='') then rtmpargs=rtmpargs||'--swfUrl "'rtmparg.swfurl'" '
			if(rtmparg.pageurl~='') then rtmpargs=rtmpargs||'--pageUrl "'rtmparg.pageurl'" '
			if(protocol='rtmp') then rtmpargs=rtmpargs||'--protocol "0" '
			if(protocol='rtmpe') then rtmpargs=rtmpargs||'--protocol "3" '
			if(rtmp_auth='on') then do
        			if(rtmparg.swfvfy~='') then do
        				if(save_auth_rtmp~='') then save_rtmp=save_auth_rtmp
					rtmpargs=rtmpargs||'--swfVfy "'rtmparg.swfvfy'" '
				end
				else do
					if(rtmparg.swfsize~='' & rtmparg.swfhash~='') then do
	        				if(save_auth_rtmp~='') then save_rtmp=save_auth_rtmp
						rtmpargs=rtmpargs||'--swfhash "'rtmparg.swfhash'" '
						rtmpargs=rtmpargs||'--swfsize "'rtmparg.swfsize'" '
					end
	       			end
			end
			rtmpargs=save_rtmp' -e 'rtmpargs'-o "'target'"'
			sayit(9,rtmpargs)
			answer=1
			do while answer=1
				address command rtmpargs
				address command 'filenote "'target'" "'inputurl'" quiet'
				if(rtmp_dlcomp='on') then answer=requestchoice('"getVideo" "RTMP download completed?*n'||translate(title,'','"')||'" "try to resume" "completed"')
				else answer=0
			end
		end
		if(~protocolcheck) then do
			sayit(3,'')
			sayit(3,'  Protocol 'protocol' is not supported.')
			address command 'delete "'target'" quiet'
			return 0
		end
		if(action='saveplay') then do
			sayit(6,'  download finished')
			sayit(6,'  try to play the saved file')
			address command rep_placeholder(play_file)
		end
		sayit(6,'  done')
	end
	if(action='play') then do
		if(protocol="mms" | protocol="http") then do
			sayit(6,'')
			sayit(6,'  try to play over stream')
			address command rep_placeholder(play_stream)
			sayit(6,'  done')
			return 0
		end
		if(pos('rtmp',protocol)~=0) then do
			if(rtmparg.url~='') then do
				sayit(6,'')
				sayit(6,'  try to play over stream')
				location=rtmparg.url
				address command rep_placeholder(play_stream)
				sayit(6,'  done')
				return 0
			end
			else do
				sayit(3,'')
/*				sayit(3,'  Protocol 'protocol' can only be saved.') */
				sayit(3,'  Sorry the stream can only be saved!')
				return 0
			end
		end
		sayit(3,'')
		sayit(3,'  Protocol 'protocol' is not supported.')
		return 0
	end
	sayit(9,'<< getvideo')
	return 0
html_chars_decode: PROCEDURE
	parse arg data
	/* char replace stems */
	str2rep.0   = 11
	str2rep.1   = '&amp;';   str2ins.1   = '&' /* this must be first */
	str2rep.2   = '&quot;';  str2ins.2   = '"'
	str2rep.3   = '&gt;';    str2ins.3   = '>'
	str2rep.4   = '&lt;';    str2ins.4   = '<'
	str2rep.5   = '&Auml;';  str2ins.5   = 'Ae'
	str2rep.6   = '&auml;';  str2ins.6   = 'ae'
	str2rep.7   = '&Ouml;';  str2ins.7   = 'Oe'
	str2rep.8   = '&ouml;';  str2ins.8   = 'oe'
	str2rep.9   = '&Uuml;';  str2ins.9   = 'Ue'
	str2rep.10  = '&uuml;';  str2ins.10  = 'ue'
	str2rep.11  = '&szlig;'; str2ins.11  = 'ss'
	i=str2rep.0
	do x=1 to 255
		i=i+1
		str2rep.i = '&#'||x||';'
		str2ins.i = d2c(x)
		i=i+1
		str2rep.i = '&#'||copies('0',3-length(x))||x||';'
		str2ins.i = d2c(x)
	end
	str2rep.0=i
	do i=1 to str2rep.0
		data=repstr(data,str2rep.i,str2ins.i)
	end
	return data
httpcode: PROCEDURE
	parse arg line
	code=''
	parse var line 'HTTP/'ver' 'code' 'dummy
	if(code='') then do
		close(tcp)
		error_parse_data()
	end
	return code
lower: PROZEDURE
	parse arg data
	data=translate(data,'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')
	return data
parse_asx:
	sayit(9,'>> parse_asx')
	parse arg uri
	parse var uri 'http://'domain'/'path
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		line=readln(tcp)
		x=0;
		do while(~eof(tcp))
			line=readln(tcp)
			sayit(9,'line='line)
			tempuri=''
			if(tempuri='') then parse var line dummy' href'dummy'"'tempuri'"'dummy
			if(tempuri='') then parse var line dummy' HREF'dummy'"'tempuri'"'dummy
			if(tempuri~='') then do;x=x+1;uri.x=tempuri;end
			if(pos('</asx>',line)~=0) then break
			if(pos('</ASX>',line)~=0) then break
		end
		close(tcp)
	end
	else error_network_timeout()
	sayit(9,'available urls:' x)
	if(x=0) then return 0
	if(x>1) then do
		rcbuttons=''
		do i=1 to x; rcbuttons=rcbuttons||' "Source 'i'"'; end
		answer=requestchoice('"getVideo" "Please select one ASX source."'rcbuttons' "cancel"')
		sayit(9,'answer='answer)
		if(answer=0) then BREAK_C()
		sayit(9,'answer='answer)
		uri.1=uri.answer
	end
	sayit(9,'asx_uri='uri.1)
	sayit(9,'<< parse_asx')
	return uri.1
parse_meta:
	sayit(9,'>> parse_meta')
	parse arg uri
	parse var uri 'http://'domain'/'path
	if open(tcp, "TCP:"domain"/80", 'RW') then do
		writeln(tcp, create_http_request('GET'))
		line=readln(tcp)
		x=0;
		do while(~eof(tcp))
			line=readln(tcp)
			sayit(9,'line='line)
			tempuri=''
			if(tempuri='') then parse var line 'url>'tempuri'</'
			if(tempuri~='') then do;x=x+1;uri.x=tempuri;end
			if(pos('</asx>',line)~=0) then break
			if(pos('</ASX>',line)~=0) then break
		end
		close(tcp)
	end
	else error_network_timeout()
	sayit(9,'available urls:' x)
	if(x=0) then return 0
	if(x>1) then do
		rcbuttons=''
		do i=1 to x; rcbuttons=rcbuttons||' "Source 'i'"'; end
		answer=requestchoice('"getVideo" "Please select one Meta source."'rcbuttons' "cancel"')
		sayit(9,'answer='answer)
		if(answer=0) then BREAK_C()
		sayit(9,'answer='answer)
		uri.1=uri.answer
	end
	sayit(9,'meta_uri='uri.1)
	sayit(9,'<< parse_meta')
	return uri.1
read_config:
	funnyordie_filetype=''
	youtube_filetype=''
	
	max_rc_buttons=''; rtmp_auth=''; rtmp_dlcomp=''
	save_http=''; save_mms=''; save_rtmp=''; save_auth_rtmp=''; play_file=''; play_stream=''
	infolevel_play=''; infolevel_save=''
	/* get pathname to program and convert into config file name */
	configFN = ''
	parse source configFN
	configFN = subword(configFN,4)
	fw = words(configFN)
	fw = wordindex(configFN,fw-1)-2
	configFN = substr(configFN,1,fw)
	fw = pos('getVideo.rexx',configFN,1) - 1
	configFN = left(configFN,fw) || 'getVideo.cfg'
	fchk=0
	if(fchk=0) then do; if(open(config,configFN,'R')) then fchk=1; end;
	if(fchk=0) then do; if(open(config,'env:getVideo.cfg','R')) then fchk=1; end;
	if(fchk=0) then do; if(open(config,'envarc:getVideo.cfg','R')) then fchk=1; end;
	if(fchk=1) then do
		do while(~eof(config))
			line=strip(readln(config))
			if(left(line,1)~=';' & line~='') then do
				if(funnyordie_filetype='') then parse var line 'funnyordie_filetype'dummy'"'funnyordie_filetype'"'
				if(youtube_filetype='') then parse var line 'youtube_filetype'dummy'"'youtube_filetype'"'
				if(max_rc_buttons='') then parse var line 'max_rc_buttons'dummy'"'max_rc_buttons'"'
				if(save_http='') then parse var line "save_http"dummy"'"save_http"'"
				if(save_mms='') then parse var line "save_mms"dummy"'"save_mms"'"
				if(save_rtmp='') then parse var line "save_rtmp"dummy"'"save_rtmp"'"
				if(save_auth_rtmp='') then parse var line "save_auth_rtmp"dummy"'"save_auth_rtmp"'"
				if(play_stream='') then parse var line "play_stream"dummy"'"play_stream"'"
				if(play_file='') then parse var line "play_file"dummy"'"play_file"'"
				if(rtmp_auth='') then parse var line 'rtmp_auth'dummy'"'rtmp_auth'"'
				if(rtmp_dlcomp='') then parse var line 'rtmp_dlcomp'dummy'"'rtmp_dlcomp'"'
				if(infolevel_play='') then parse var line 'infolevel_play'dummy'"'infolevel_play'"'
				if(infolevel_save='') then parse var line 'infolevel_save'dummy'"'infolevel_save'"'
			end
		end
		close(config)
	end
	if(max_rc_buttons='') then max_rc_buttons=10
	if(save_http='') then save_http='wget -c -O "%d" "%s"'
	if(save_mms='') then save_mms='mplayer >nil: -dumpfile "%d" -dumpstream "%S" -quiet'
	if(save_rtmp='') then save_rtmp='rtmpdump'
	if(rtmp_auth='') then rtmp_auth='on'
	if(rtmp_dlcomp='') then rtmp_dlcomp='on'
	if(play_file='') then play_file='mplayer >nil: *>nil: "%d" -quiet'
	if(play_stream='') then play_stream='mplayer >nil: *>nil: "%S" -quiet'
	if(action='play') then do; if(infolevel_play='') then infolevel=3; else infolevel=infolevel_play; end
	if(pos('save',action)~=0) then do; if(infolevel_save='') then infolevel=6; else infolevel=infolevel_save; end
	if(action~='play' & pos('save',action)=0) then infolevel=6
	if(open(dummy,'env:getvideo_debug')) then do
		close(dummy)
		infolevel=9
	end
	sayit(9,'<< read_config')
	return 0
rep_placeholder:
	sayit(9,'>> rep_placeholder')
	parse arg phdata
	sayit(9,'phdata='phdata)
	phdata=repstr(phdata,'%d',target)	/* destination path and filename */
	phdata=repstr(phdata,'%s',pflocation)	/* streamurl without protocol */
	phdata=repstr(phdata,'%S',location)	/* streamurl with protocol */
	sayit(9,'phdata='phdata)
	sayit(9,'<< rep_placeholder')
	return phdata
repstr: PROCEDURE
	parse arg data,str2rep,str2ins
	output=''
	do while(pos(str2rep,data)~=0)
		position=pos(str2rep,data)-1
		output=output||substr(data,1,position)||str2ins
		data=delstr(data,1,position+length(str2rep))
	end
	return output||data
requestchoice: PROCEDURE
	parse arg argumente
	rcfileid='t:getvideo_'||time('s')
	address command 'RequestChoice > 'rcfileid' 'argumente
	open(getvideo,rcfileid); answer=readln(getvideo); close(getvideo); address command 'delete 'rcfileid' quiet'
	return answer
rtmpdumpinit:
	rtmpargs=''
	rtmparg.url=''
	rtmparg.rtmp=''
	rtmparg.host=''
	rtmparg.path=''
	rtmparg.app=''
	rtmparg.swfurl=''
	rtmparg.swfsize=''
	rtmparg.swfhash=''
	rtmparg.pageurl=''
	rtmparg.swfvfy=''
	return 0
sayit:
	parse arg level,msg
	if(infolevel>=level) then say msg
	return 0
seconds_hra: PROCEDURE
	parse arg s
	output=''
	h=trunc(s/3600);
	if(h>0) then do; s=s-(h*3600); if(length(h)=1) then do; h='0'||h; end; output=output||h':'; end
	m=trunc(s/60);
	s=s-(m*60); if(length(m)=1) then do; m='0'||m; end;  output=output||m':'
	if(length(s)=1) then do; s='0'||s; end; output=output||s
	return output
targetcheck:
	if(open(dummy,target)) then do
		close(dummy)
		answer=requestchoice('"getVideo - File exists" "*n  File: *"'target'*"  *n  allready exists." "try to resume" "save as..." "overwrite" "cancel"')
		sayit(9,'answer='answer)
		if(answer=1) then return target
		if(answer=2) then do
			if(length(downloaddir)~=0) then dldir='drawer 'downloaddir
			else dldir=''
			address command 'requestfile > env:getvideo title "getVideo - save as..." 'dldir' file 'file'.'extension' pattern #?.'extension' noicons'
			open(getvideo,'env:getvideo')
			answer=readln(getvideo)
			answer=strip(translate(answer,'','"'))
			close(getvideo)
			address command 'delete env:getvideo quiet'
			if(answer='') then BREAK_C()
			else target=answer
		end
		if(answer=3) then address command 'delete "'target'" force quiet'
		if(answer=0) then BREAK_C()
	end
	if(pos('rtmp',protocol)=0) then address command 'touch "'target'"'
	return target
title_to_filename: PROCEDURE
	parse arg title
	str2rep.0   = 27
	str2rep.1   = ':'; str2ins.1   = ''
	str2rep.2   = '/'; str2ins.2   = '_'
	str2rep.3   = ' '; str2ins.3   = '_'
	str2rep.4   = '!'; str2ins.4   = ''
	str2rep.5   = '?'; str2ins.5   = ''
	str2rep.6   = "'"; str2ins.6   = ''
	str2rep.7   = '"'; str2ins.7   = ''
	str2rep.8   = '|'; str2ins.8   = '_'
	str2rep.9   = '*'; str2ins.9   = '_'
	str2rep.10  = '('; str2ins.10  = '_'
	str2rep.11  = ')'; str2ins.11  = '_'
	str2rep.12  = '['; str2ins.12  = '_'
	str2rep.13  = ']'; str2ins.13  = '_'
	str2rep.14  = '%'; str2ins.14  = ''
	str2rep.15  = '$'; str2ins.15  = ''
	str2rep.16  = '<'; str2ins.16  = '_'
	str2rep.17  = '>'; str2ins.17  = '_'
	str2rep.18  = '#'; str2ins.18  = ''
	str2rep.19  = '~'; str2ins.19  = ''
	str2rep.20  = ';'; str2ins.20  = ''
	str2rep.21  = 'ä'; str2ins.21  = 'ae'
	str2rep.22  = 'ö'; str2ins.22  = 'oe'
	str2rep.23  = 'ü'; str2ins.23  = 'ue'
	str2rep.24  = 'Ä'; str2ins.24  = 'AE'
	str2rep.25  = 'Ö'; str2ins.25  = 'OE'
	str2rep.26  = 'Ü'; str2ins.26  = 'UE'
	str2rep.27  = 'ß'; str2ins.27  = 'ss'
	i = str2rep.0
	do x = 1 to 255
		i=i+1
		str2rep.i = d2c(x)
		str2ins.i = ''
		if(x==31) then x=127
	end
	str2rep.0 = i
	do i = 1 to str2rep.0
		title=repstr(title,str2rep.i,str2ins.i)
	end
	maxtitlelength = 80-4 /* amiga dos can handle filenames with 107 chars */
	if(length(title)>maxtitlelength) then title = substr(title,1,maxtitlelength)
	if(title='') then title = 'unknown_title'
	return title
unescape: PROCEDURE
	parse arg uri
	do i = 32 to 255
		str2rep = '%'||d2x(i)
		str2ins = d2c(i)
		uri=repstr(uri,str2rep,str2ins)
		str2rep = translate(str2rep,'abcdef','ABCDEF')
		uri=repstr(uri,str2rep,str2ins)
	end
	return strip(uri)
unicode_to_latin1: PROCEDURE
	parse arg uri
	do i = 32 to 127
		str2rep = '\u00'||substr(c2x(b2c(substr(c2b(d2c(i)),1,4))),2,1)||substr(c2x(b2c(substr(c2b(d2c(i)),5,4))),2,1)
		str2rep = translate(str2rep,'abcdef','ABCDEF')
		str2ins = d2c(i)
		uri=repstr(uri,str2rep,str2ins)
	end
	return strip(uri)
usage:
	sayit(3,'')
	sayit(3,'  usage: getvideo "url" action "download path"')
	expected_url()
	exit
utf8_to_latin1: PROCEDURE
	parse arg title
	str2rep.0   = 9
	str2rep.1   = 'Ã„'; str2ins.1  = 'Ä'
	str2rep.2   = 'Ã–'; str2ins.2  = 'Ö'
	str2rep.3   = 'Ãœ'; str2ins.3  = 'Ü'
	str2rep.4   = 'Ã¤'; str2ins.4  = 'ä'
	str2rep.5   = 'Ã¶'; str2ins.5  = 'ö'
	str2rep.6   = 'Ã¼'; str2ins.6  = 'ü'
	str2rep.7   = 'Â©'; str2ins.7  = '©'
	str2rep.8   = 'ÃŸ'; str2ins.8  = 'ß'
	str2rep.9   = 'Â´'; str2ins.9  = '´'
	do i = 1 to str2rep.0
		title=repstr(title,str2rep.i,str2ins.i)
	end
	if(title='') then title = 'unknown_title'
	return title
