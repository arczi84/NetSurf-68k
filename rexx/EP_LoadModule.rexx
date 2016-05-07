/* Start EaglePlayer if not allready started. Optional: load a given file */

options
parse arg player filename

/*EaglePlayer = 'SYS:Utilities/EaglePlayer/EaglePlayer'*/

EaglePlayer = player
filename=strip(filename)

if show('P','rexx_EP') == '0' then do

  address COMMAND 'Run ' EaglePlayer Loadmodule " " filename

end  
else do

address 'rexx_EP' Loadmodule filename

end
