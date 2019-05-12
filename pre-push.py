#!C:/Program\ Files\ (x86)/Microsoft\ Visual\ Studio/Shared/Python36_64/python.exe                                                        

from __future__ import print_function
import pickle
import os.path
import shutil
import os
import errno
import datetime
import stat
from time import strftime
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from apiclient.http import MediaFileUpload
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive

# If modifying these scopes, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/drive']
CLIENT_SECRET_FILE =  'client_secrets.json'
FILE_NAME = 'Content.zip'
ARCHIVE_DIRECTORY = '../../Content'
PUSH_DATE_FILE_NAME = '../../LastPushDate.txt'

def get_drive_service():
    """Shows basic usage of the Drive v3 API.
    Prints the names and ids of the first 10 files the user has access to.
    """
    creds = None
    # The file token.pickle stores the user's access and refresh tokens, and is
    # created automatically when the authorization flow completes for the first
    # time.
    if os.path.exists('token.pickle'):
        with open('token.pickle', 'rb') as token:
            creds = pickle.load(token)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                CLIENT_SECRET_FILE, SCOPES)
            creds = flow.run_local_server()
        # Save the credentials for the next run
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)

    service = build('drive', 'v3', credentials=creds)

    return service


def make_zip(output_file_name, dir_name):
    shutil.make_archive(output_file_name, 'zip', dir_name)

def get_file_id(drive_service, file_name):
    search_query = 'name=' + '"' + file_name + '"'
    response = drive_service.files().list(q=search_query, 
                                          spaces='drive', 
                                          fields='files(id)').execute()

    result = []
    for files_id in response.get('files', []):
        result.append(files_id.get('id'))
    return result


def delete_files(files_id, drive_service):
    for file_id in files_id:
        drive_service.files().delete(fileId = file_id).execute()


# File upload works through PyDrive library, cause it is a bit complicated to
# upload large files through bare google API. I've tried to use PyDrive for the rest
# of functionality but it worked pretty bad. So I've end up with this mix :(
def upload_file(file_name):
    gauth = GoogleAuth()
    gauth.LocalWebserverAuth()

    drive = GoogleDrive(gauth)

    file = drive.CreateFile({'title' : file_name})
    file.SetContentFile(file_name)
    file.Upload()


def get_mod_date(directory):

    modified = 0
    rootdir = os.path.join(os.getcwd(), directory)

    for root, sub_folders, files in os.walk(rootdir):
        for file in files:
            try:
                unix_modified_time = os.stat(os.path.join(root, file))[stat.ST_MTIME]
                modified = max(modified, unix_modified_time)
            except:
                pass

    return modified


def get_last_push_date():

    if not os.path.exists(PUSH_DATE_FILE_NAME):
        with open(PUSH_DATE_FILE_NAME, 'w+') as f:
            return 0

    with open(PUSH_DATE_FILE_NAME, 'r') as file:
        first_line = file.readline()
        if not first_line:
            return 0

        return int(first_line)


def is_push_needed():
    return get_mod_date(ARCHIVE_DIRECTORY) > get_last_push_date()


def update_date_file():
    with open(PUSH_DATE_FILE_NAME, 'w') as file:
        file.write(str(get_mod_date(ARCHIVE_DIRECTORY)))


def main():
    if not is_push_needed():
        return 0

    # Remove old archive file
    try:
        os.remove(FILE_NAME)
    except OSError as e:
        if e.errno != errno.ENOENT: # errno.ENOENT = no such file or directory
            raise # re-raise exception if a different error occurred

    make_zip(FILE_NAME.split('.')[0], ARCHIVE_DIRECTORY)

    service = get_drive_service()
    # Clean up drive from old files
    files_id = get_file_id(service, FILE_NAME)
    delete_files(files_id, service)
    # Upload new file
    upload_file(FILE_NAME)

    update_date_file()

    return 0

if __name__ == '__main__':
    main()
