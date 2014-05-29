
import Pyro4

class GreetingMaker(object):
    def get_fortune(self, name):
        return "Hello, {0}. Here is your fortune message:\n" \
               "Behold the warranty -- the bold print giveth and the fine print taketh away.".format(name)

greeting_maker=GreetingMaker()

daemon=Pyro4.Daemon()                 # make a Pyro daemon
uri=daemon.register(greeting_maker)   # register the greeting object as a Pyro object

print "Ready. Object uri =", uri      # print the uri so we can use it in the client later
daemon.requestLoop()                  # start the event loop of the server to wait for calls
