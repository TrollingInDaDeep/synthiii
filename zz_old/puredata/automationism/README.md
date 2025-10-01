aaaauso basically:
patchmatrix.pd isch numä ds ding wo d sachä zämä patched wärdä, äs kontrolliert intern ä subpatch
synth heisst dä. dert wärdä connections connected und disconnected gstüret vo patchmatrix.pd

im subpatch synth chamä leider keni modul derzuetue, deswägä immer d änderige mache in main.pd, vo dert när
ctrl+a, ctrl+c -> i subpatch "synth" in patchmatrix.pd, dert ctrl+a, del, ctrl+v
weis gad nid wimäs besser macht wüu in patchmatrix.pd dr connect befäu geit iwie nur mit subpatches...

connect funktioniert immer mit dr ID vo de modul, aber di steit niene, ds isch eif d nummerierig 0-basiert, äuä ir hinzuefüegigs
reihefolg...
da äuä mau äs sheet machä wo ds drinnä steit was was isch.

osc3 15
Kick1 49
sinebank 55

 ### English ###
Patchmatrix.pd
buttons and logic that connects and disconnects modules of automationism.
Automationism is stored in a subpatch within PatchMatrix.pd

Adding automationism modules doesn't work in a subpatch, so to make changes:
1. edit and test in main.pd
2. copy all (ctrl+a, ctrl+c)
3. go to PatchMatrix.pd
4. go into the subpatch "synth"
5. delete all and paste in the contents of Main (ctrl+a, del, ctrl+v)

connect command works as follows:
connect <source moduleID> <outlet ID> <target module ID> <inlet-ID>

module ID is written nowhere, its 0-based numbering in order of adding them.
opening the .pd file in notepad can help to identify, there it says the id of every component added