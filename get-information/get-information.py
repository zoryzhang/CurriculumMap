'''
This part of the python code collects the course information on the HKUST website

https://prog-crs.ust.hk/ugcourse

and provides two Excel saving format (.xls) and (.csv).

The collected data will be used for mapping.

'''

import re
import csv
import urllib.request
from bs4 import BeautifulSoup
from tqdm import tqdm

#for .xls file you should use

'''
import xlwt
url = 'https://prog-crs.ust.hk/ugcourse/2022-23'
html = urllib.request.urlopen(url).read()
soup = BeautifulSoup(html, "lxml")
'''

#special case detector (abandoned)
def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass

    try:
        import unicodedata
        unicodedata.numeric(s)
        return True
    except (TypeError, ValueError):
        pass

    return False

def get_content(content):
    # content is a string
    # badstring
    content = content.replace("&amp;", "&")
    # content = content.replace("&quot;", "$")
    # content = content.replace("&apos;", "#")
    content = content.replace("“","$")
    content = content.replace("”","$")
    content = content.replace("\"", "$")
    content = content.replace("‘","#")
    content = content.replace("’","#")
    content = content.replace("\'","#")
    content = content.replace("&lt;", "<")
    content = content.replace("&gt;", ">")
    # csv specific
    content = content.replace(",", "@")
    content = content.strip('\xa0')
    return content

def get_subject_urls(soup):
    subject_prefix = [ chr(ord('A') + i) for i in range(26)]
    subjects = []
    subject_urls = []
    for prefix in tqdm(subject_prefix, desc="Crawling Subjects"):
        found_prefix = soup.find_all("li", {"class" : "subject subject-prefix-" + prefix})
        for li in found_prefix:
            subject_urls.append("https://prog-crs.hkust.edu.hk" + str(li.a.get("href")))
            subjects.append(str(li.div.text))
    return subjects, subject_urls

def get_courses(subject_urls):
    courses = []
    for url in tqdm(subject_urls, desc="Crawling Courses"):
        #use soup to find "li" Label and specified classes
        html = urllib.request.urlopen(url).read()
        soup = BeautifulSoup(html, "lxml")
        courses.extend(soup.find_all("li", {"class": "crse accordion-item"}))
    return courses

def process_courses(courses):
    all_row = []

    # Regular Expression
    reg = re.compile("<[^>]*>")

    for course in tqdm(courses, desc="Processing Courses"):
        row = [] # Operation row
        st = reg.sub('$', str(course))  # Regular Expression Replace
        st = st.strip()
        #print(st)
        entries = st.split('$')
        entries = list(filter(None, entries))
        #print(entries)
        row.append(get_content(entries[0])) # Code
        row.append(get_content(entries[1])) # Name
        row.append(get_content(entries[2])) # Credits

        #Prerequisite(s)
        cnt = entries.count('Prerequisite(s)')
        if cnt != 0:
            index = entries.index('Prerequisite(s)')
            row.append(get_content(entries[index + 1]))
        else:
            row.append('')

        #Exclusion(s)
        cnt = entries.count('Exclusion(s)')
        if cnt != 0:
            index = entries.index('Exclusion(s)')
            row.append(get_content(entries[index + 1]))
        else:
            row.append('')

        #Corequisite(s)
        cnt = entries.count('Corequisite(s)')
        if cnt != 0:
            index = entries.index('Corequisite(s)')
            row.append(get_content(entries[index + 1]))
        else:
            row.append('')

        #Co-list with
        cnt = entries.count('Co-list with')
        if cnt != 0:
            index = entries.index('Co-list with')
            row.append(get_content(entries[index + 1]))
        else:
            row.append('')

        #Mode of Delivery
        cnt = entries.count('Mode of Delivery')
        if cnt != 0:
            index = entries.index('Mode of Delivery')
            row.append(get_content(entries[index + 1]))
        else:
            row.append('')

        #Previous Course Code(s)
        cnt = entries.count('Previous Course Code(s)')
        if cnt != 0:
            index = entries.index('Previous Course Code(s)')
            row.append(get_content(entries[index + 1]))
        else:
            row.append('')

        #Alternate code(s)
        cnt = entries.count('Alternate code(s)')
        if cnt != 0:
            index = entries.index('Alternate code(s)')
            row.append(get_content(entries[index + 1]))
        else:
            row.append('')

        #Description
        cnt = entries.count('Description')
        if cnt != 0:
            index = entries.index('Description')
            temp = get_content(entries[index + 1])
            row.append(temp)
        else:
            row.append('')

        row.append("@")
        all_row.append(row)

    return all_row

def output_csv(path, all_row, num_courses, num_subjects):
    #supporting Chinese by 'utf-8-sig'
    with open(path,'w',newline = '', encoding="utf-8-sig") as file_csv:
        csv_write = csv.writer(file_csv)
        csv_stastic_head = [str(num_courses), str(num_subjects),"","","","","","","", "@"]
        csv_write.writerow(csv_stastic_head)
        csv_head = ['Course Code', 'Course Name', 'Course Credits', 'Prerequisite(s)', 'Exclusion(s)','Corequisite(s)','Co-list with','Mode of Delivery','Previous Course Code(s)','Alternate code(s)', "@"]
        csv_write.writerow(csv_head)
        for sub_list in all_row:
            csv_write.writerow(sub_list)

def main():
    # reserved for special case with '\xa0'
    '''
    temp = temp.strip('\xa0')
    temp = temp.strip(';')
    '''

    #a special case may be used in the following sections

    '''
    if t_list and t_list[0] != '': #special cases
        if is_number(t_list[0][0]) and len(t_list[0]) <= 10:
            row.append(t_list[0])
    '''

    print("Loading HTML")
    base_url = 'https://prog-crs.ust.hk/ugcourse/'
    html = urllib.request.urlopen(base_url).read()
    soup = BeautifulSoup(html, "lxml")

    subjects, subject_urls = get_subject_urls(soup)
    num_subjects = len(subjects)

    # print(subject_urls)
    # print(subjects)

    courses = get_courses(subject_urls)
    num_courses = len(courses)

    all_row = process_courses(courses) + [ [s] for s in subjects ]

    output_csv("courses.csv", all_row, num_courses, num_subjects)

if __name__ == '__main__':
    main()

"""
"""
