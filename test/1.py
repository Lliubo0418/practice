user = "seven"
passwd = 123
username = input("please the enter user:")
password = int(input("please the enter passwd:"))
if username == user and password == passwd:
    print("logon successfull")
else:
    print("logon failed")