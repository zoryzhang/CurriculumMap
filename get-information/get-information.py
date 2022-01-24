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

#for .xls file you should use

''' 
import xlwt
url = 'https://prog-crs.ust.hk/ugcourse/2021-22/COMP'
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
    #content = content.replace("&quot;", "$")
    #content = content.replace("&apos;", "#")
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

def main():
    print('this message is from main function')
    # print(soup.prettify())

    # Regular Expression
    reg = re.compile("<[^>]*>")

    # reserved for special case with '\xa0'
    '''
    temp = temp.strip('\xa0')
    temp = temp.strip(';')
    '''

    all_row = []

    #a special case may be used in the following sections
        
    ''' 
    if t_list and t_list[0] != '': #special cases
        if is_number(t_list[0][0]) and len(t_list[0]) <= 10:
            row.append(t_list[0])

    '''

    course_abbr = ["ACCT","AESF","AIAA","AMAT","BIBU","BIEN","BSBE","BTEC","CBME","CENG","CHEM","CHMS","CIEM","CIVL","CMAA","COMP","CPEG","CSIT","DASC","DBAP","DSAA","DSCT","ECON","EEMT","EESM","ELEC","EMBA","ENEG","ENGG","ENTR","ENVR","ENVS","EOAS","EVNG","EVSM","FINA","FTEC","FUNH","GBUS","GFIN","GNED","HART","HLTH","HMMA","HUMA","IBTM","IDPO","IEDA","IIMP","IMBA","INFH","INTR","IOTA","IPEN","ISDN","ISOM","JEVE","LABU","LANG","LIFS","MAED","MAFS","MARK","MASS","MATH","MECH","MESF","MFIT","MGCS","MGMT","MICS","MILE","MIMT","MSBD","MSDM","MTLE","NANO","OCES","PDEV","PHYS","PPOL","RMBI","ROAS","SBMT","SCIE","SEEN","SHSS","SMMG","SOCH","SOSC","SUST","SYSH","TEMG","UGOD","UROP","WBBA"]
    #course_abbr = ["ACCT"]
    base_url = ['https://prog-crs.ust.hk/ugcourse/2021-22/']

    
    for abbr in course_abbr:
        #use soup to find "li" Label and specified classes
        real_url = base_url[0] + abbr
        html = urllib.request.urlopen(real_url).read()
        soup = BeautifulSoup(html, "lxml")
        header_find = soup.find_all("li", {"class": "crse accordion-item"})


        for child in header_find:
            row = [] # Operation row
            st = reg.sub('$', str(child))  # Regular Expression Replace
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
                #print("BEF : " + entries[index + 1])
                #print()
                temp = get_content(entries[index + 1])
                row.append(temp)
                #print("DESC : " + temp)
                #print() 
                #temp_list = temp_list.split(".")
                #temp = temp_list[0]
                """
                temp = temp.replace("\"","")
                temp = temp.replace("“","")
                temp = temp.replace("”","")
                temp = temp.replace("‘","")
                temp = temp.replace("’","")
                temp = temp.replace("‐","")
                #temp = temp + "......(more on ustspace)" 
                """
            else:
                row.append('')

            row.append("@")
            all_row.append(row)


    all_row_length = len(all_row)
    path = "courses.csv"

    #supporting Chinese by 'utf-8-sig'
    with open(path,'w',newline = '', encoding="utf-8-sig") as file_csv:
        csv_write = csv.writer(file_csv)
        csv_stastic_head = ["Stastic_use_course_num = ", str(all_row_length),"","","","","","","","", "@"]
        csv_write.writerow(csv_stastic_head)
        csv_head = ['Course Code', 'Course Name', 'Course Credits', 'Prerequisite(s)', 'Exclusion(s)','Corequisite(s)','Co-list with','Mode of Delivery','Previous Course Code(s)','Alternate code(s)', "@"]
        csv_write.writerow(csv_head)
        for sub_list in all_row:
            csv_write.writerow(sub_list)

if __name__ == '__main__':
    main()

