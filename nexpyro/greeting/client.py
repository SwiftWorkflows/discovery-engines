import Pyro4

uri=raw_input("What is the Pyro uri of the greeting object? ").strip()
name=raw_input("What is your name? ").strip()

greeting_maker=Pyro4.Proxy(uri)          # get a Pyro proxy to the greeting object
print greeting_maker.get_fortune(name)   # call method normally
