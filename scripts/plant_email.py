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
    moist_msg = "Moderate"
    if moist < 0:
        moist_msg = "Deficient"
    elif moist == 0:
        moist_msg = "Moderate"
    else:
        moist_msg = "Excessive"

    light_msg = "Moderate"
    if light < 0:
        light_msg = "Deficient"
    elif light == 0:
        light_msg = "Moderate"
    else:
        light_msg = "Excessive"

    return moist_msg, light_msg

def main():
    # Interactive version
    # init_email()
    # print(recipient_address)
    # print(type(recipient_address))


    # Command prompt version
    parser = argparse.ArgumentParser(description='Generate email notifying the user about the plant\'s condition.')
    parser.add_argument('-e', '--email', action='store', type=str, required=True, help="Email of the client.", dest='email')
    parser.add_argument('-m', '--moisture', action='store', default=1, type=int, choices=[-1, 0, 1], help="The plant's moisture level [-1: Deficient, 0: Moderate, 1: Excessive].", dest='moist')
    parser.add_argument('-l', '--light', action='store', default=0, type=int, choices = [-1, 0, 1], help="The plant's light level [-1: Deficient, 0: Moderate, 1: Excessive].", dest='light')
    parser.add_argument('-s', '--sample', action='store', default=False, type=bool, help="Is this a sample email?", dest='sample')
    
    args = parser.parse_args()

    # Set email
    email = args.email
    set_email_address(email)

    headings = ""
    message = ""
    if args.sample:
        headings = "[Automatic-Plant-Care] This is a sample email!"
        message = "This is a sample email!\n\nPlease do not reply to this email.\n\nRegards,\nPlant Care Developer Team"
    else:
        # Extract sensor value for the email's message
        moist, light = analyze_sensor_val(args.moist, args.light)
        headings = "[Automatic-Plant-Care] Your plant\'s health is in critical condition!"
        message = "Plant\'s health summary\n\tMoisture level: {}\n\tLight level: {}\n\nRegards,\nPlant Care Developer Team".format(moist, light)

    set_message(headings, message)
    send_email()
    
if __name__ == '__main__':
    main()
