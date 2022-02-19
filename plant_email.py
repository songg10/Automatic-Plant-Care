from smtplib import SMTP

MAX_LENGTH=2048

recipient_address=None
sender_address='plant.care.alert@gmail.com'
sender_pw='plantcare433!'
txt='testing'
my_email='sha173@sfu.ca'

# def set_email_address(email):
#     global email_adress
#     email_adress=email  

# def init_email():
#     while 1:
#         email=input('Input your email address: ')
#         confirm=float(input('Email entered is: {}\nIs this the correct email? (y/n): '.format(email)))
#         while confirm != 'y' and confirm != 'Y' and confirm != 'n' and confirm != 'N':
#             confirm=input("Enter \"y\" for yes, \"n\" for no: ")
#             if confirm == 'y' or confirm == 'Y':
#                 set_email_address(email)
#                 return

def send_email():
    server=SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login(sender_address, sender_pw)
    SMTP.sendmail(self=server, from_addr=sender_address, to_addrs=my_email, msg=txt)
    print('sent email.')
    server.quit()

def main():
    send_email()
    
if __name__ == '__main__':
    main()