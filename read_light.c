from glob import glob
from smtplib import SMTP


MAX_LENGTH=2048

recipient_address=None
sender_address='plant.care.alert@gmail.com'
sender_pw='plantcare433!'

def set_email_address(email):
    global recipient_address
    recipient_address=email  

def init_email():
    while 1:
        email=input('Input your email address: ')
        print('Email entered is: ', email)
        confirm=input('Is this the correct email? (y/n): ')
        while (confirm != 'y' and confirm != 'Y' and confirm != 'n' and confirm != 'N'):
            confirm=input("Enter \"y\" for yes, \"n\" for no: ")
        if confirm == 'y' or confirm == 'Y':
            set_email_address(email)
            return

def set_message(subject, text):
    global message
    message = 'Subject: {}\n\n{}'.format(subject, text)

def send_email():
    server=SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login(sender_address, sender_pw)
    SMTP.sendmail(self=server, from_addr=sender_address, to_addrs=recipient_address, msg=message)
    print('sent email.')
    server.quit()

def main():
    init_email()
    set_message('testing', 'testing 123')
    send_email()
    
if __name__ == '__main__':
    main()
