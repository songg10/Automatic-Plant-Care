# from glob import glob
from smtplib import SMTP
import argparse

MAX_LENGTH=2048

recipient_address=None
message = None
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
    print('Email sent.')
    server.quit()

def analyze_sensor_val(moist, light):
    moist_msg = "Healthy" if moist == 1 else "Unhealthy"

    light_msg = None
    if light < 0:
        light_msg = "Low level of sun light"
    elif light == 0:
        light_msg = "Appropriate Level of sun light"
    else:
        light_msg = "Too much sun light"

    return moist_msg, light_msg

def main():
    # Interactive version
    # init_email()
    # print(recipient_address)
    # print(type(recipient_address))


    # Command prompt version
    parser = argparse.ArgumentParser(description='Generate email notifying the user about the plant\'s condition.')
    parser.add_argument('-e', '--email', action='store', type=str, required=True, help="Email of the client.", dest='email')
    parser.add_argument('-m', '--moisture', action='store', default=1, type=int, choices=[0, 1], help="The plant's moisture level [0: unhealthy, 1: healthy].", dest='moist')
    parser.add_argument('-l', '--light', action='store', default=0, type=int, choices = [-1, 0, 1], help="The plant's light level [-1: Low light, 0: Medium light, 1: High light].", dest='light')
    
    args = parser.parse_args()
    email = args.email

    # # Set email
    set_email_address(email)

    # Extract sensor value for the email's message
    moist, light = analyze_sensor_val(args.moist, args.light)
    message = "Plant\'s health summary\n\
    Moisture level: {}\n\
    Light level: {}\n".format(moist, light)

    set_message('Your plant\'s health is in critical condition!', message)
    send_email()
    
if __name__ == '__main__':
    main()
