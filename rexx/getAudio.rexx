/*
Play audio with player 
*/

options results
SIGNAL ON BREAK_C
parse arg url action downloaddir
;url=strip(strip(url),'B','"')
/*action=lower(strip(strip(action),'B','"'))*/

address command 'ram:'
sayit(9,'Input=')
sayit(6,'')
sayit(6,'  [getAudio] v0.01')
if(action~='save' & action~='play' & action~='saveplay') then do
	answer=requestchoice('"getAudio" "What will you do?" "Play" "Save" "Save & Play" "Cancel"')
	if(answer=0) then BREAK_C()
	if(answer=1) then action='play'
	if(answer=2) then action='save'
	if(answer=3) then action='saveplay'
end
;main(url)
;exit
