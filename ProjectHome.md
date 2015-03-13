Makestatus comprises of plasmoid(applet), D-Bus enabled dataengine and a make wrapper called makeinvoker which acts as an informer to dataengine using d-bus calls.

When makeinvoker is ran in place of make, it collect the output of make and relays the information to MakeStatus dataengine using d-bus. The MakeStatus applet then displays this information in a simple way.