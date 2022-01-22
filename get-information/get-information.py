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

    real_list = ["ACCT","AESF","AIAA","AMAT","BIBU","BIEN","BSBE","BTEC","CBME","CENG","CHEM","CHMS","CIEM","CIVL","CMAA","COMP","CPEG","CSIT","DASC","DBAP","DSAA","DSCT","ECON","EEMT","EESM","ELEC","EMBA","ENEG","ENGG","ENTR","ENVR","ENVS","EOAS","EVNG","EVSM","FINA","FTEC","FUNH","GBUS","GFIN","GNED","HART","HLTH","HMMA","HUMA","IBTM","IDPO","IEDA","IIMP","IMBA","INFH","INTR","IOTA","IPEN","ISDN","ISOM","JEVE","LABU","LANG","LIFS","MAED","MAFS","MARK","MASS","MATH","MECH","MESF","MFIT","MGCS","MGMT","MICS","MILE","MIMT","MSBD","MSDM","MTLE","NANO","OCES","PDEV","PHYS","PPOL","RMBI","ROAS","SBMT","SCIE","SEEN","SHSS","SMMG","SOCH","SOSC","SUST","SYSH","TEMG","UGOD","UROP","WBBA"]
    base_url = ['https://prog-crs.ust.hk/ugcourse/2021-22/']

    
    for it_A in real_list:
        #use soup to find "li" Label and specified classes
        real_url = base_url[0] + it_A
        html = urllib.request.urlopen(real_url).read()
        soup = BeautifulSoup(html, "lxml")
        header_find = soup.find_all("li", {"class": "crse accordion-item"})


        for child in header_find:
            row = [] # Operation row
            st = reg.sub('$', str(child))  # Regular Expression Replace
            st = st.strip()
            t_list = st.split('$')
            t_list = list(filter(None, t_list))
            #print(t_list)
            row.append(t_list[0]) # Code
            row.append(t_list[1]) # Name
            row.append(t_list[2]) # Credits

            #Prerequisite(s)
            cnt = t_list.count('Prerequisite(s)')
            if cnt != 0:
                index = t_list.index('Prerequisite(s)')
                temp = t_list[index + 1]
                temp = temp.strip('\xa0')
                row.append(temp)
            else:
                row.append('')

            #Exclusion(s)
            cnt = t_list.count('Exclusion(s)')
            if cnt != 0:
                index = t_list.index('Exclusion(s)')
                temp = t_list[index + 1]
                temp = temp.strip('\xa0')
                row.append(temp)
            else:
                row.append('')

            #Corequisite(s)
            cnt = t_list.count('Corequisite(s)')
            if cnt != 0:
                index = t_list.index('Corequisite(s)')
                temp = t_list[index + 1]
                temp = temp.strip('\xa0')
                row.append(temp)
            else:
                row.append('')

            #Co-list with
            cnt = t_list.count('Co-list with')
            if cnt != 0:
                index = t_list.index('Co-list with')
                temp = t_list[index + 1]
                temp = temp.strip('\xa0')
                row.append(temp)
            else:
                row.append('')

            #Mode of Delivery
            cnt = t_list.count('Mode of Delivery')
            if cnt != 0:
                index = t_list.index('Mode of Delivery')
                temp = t_list[index + 1]
                temp = temp.strip('\xa0')
                row.append(temp)
            else:
                row.append('')

            #Previous Course Code(s)
            cnt = t_list.count('Previous Course Code(s)')
            if cnt != 0:
                index = t_list.index('Previous Course Code(s)')
                temp = t_list[index + 1]
                temp = temp.strip('\xa0')
                row.append(temp)
            else:
                row.append('')

            #Alternate code(s)
            cnt = t_list.count('Alternate code(s)')
            if cnt != 0:
                index = t_list.index('Alternate code(s)')
                temp = t_list[index + 1]
                temp = temp.strip('\xa0')
                row.append(temp)
            else:
                row.append('')

            #Description
            cnt = t_list.count('Description')
            if cnt != 0:
                index = t_list.index('Description')
                temp_list = t_list[index + 1]
                temp_list = temp_list.strip('\xa0')
                temp = temp_list
                #temp_list = temp_list.split(".")
                #temp = temp_list[0]
                temp = temp.replace("\"","")
                temp = temp.replace("“","")
                temp = temp.replace("”","")
                temp = temp.replace("‘","")
                temp = temp.replace("’","")
                temp = temp.replace("‐","")
                #temp = temp + "......(more on ustspace)" 
                row.append(temp)
            else:
                row.append('')

            all_row.append(row)


    #this is the .xls solution but diffcult to be used in the following codes
    ''' 
    all_row_length = len(all_row)
    book = xlwt.Workbook(encoding='utf-8')
    sheet = book.add_sheet('test', cell_overwrite_ok=True)
    print(all_row_length)
    sheet.write(0,0,"Static_use_course_num = ")
    sheet.write(0,1,all_row_length)
    head = ['Course Code', 'Course Name', 'Course Credits', 'Prerequisite(s)', 'Exclusion(s)','Corequisite(s)','Co-list with','Mode of Delivery','Previous Course Code(s)','Alternate code(s)']
    for h in range(len(head)):
        sheet.write(1, h, head[h])
    now_row = 2  # write from first row
    for course_infolist in all_row:
        now_col = 0
        for info_item in course_infolist:
            sheet.write(now_row, now_col, info_item)  # cell_overwrite_ok = True
            now_col = now_col + 1
        now_row = now_row + 1
    book.save('D:test_data.xls')
    '''

    all_row_length = len(all_row)
    path = "courses.csv"

    #supporting Chinese by 'utf-8-sig'
    with open(path,'w',newline = '', encoding="utf-8-sig") as file_csv:
        csv_write = csv.writer(file_csv)
        csv_stastic_head = ["Stastic_use_course_num = ", str(all_row_length),"","","","","","","",""]
        csv_write.writerow(csv_stastic_head)
        csv_head = ['Course Code', 'Course Name', 'Course Credits', 'Prerequisite(s)', 'Exclusion(s)','Corequisite(s)','Co-list with','Mode of Delivery','Previous Course Code(s)','Alternate code(s)']
        csv_write.writerow(csv_head)
        for sub_list in all_row:
            csv_write.writerow(sub_list)

if __name__ == '__main__':
    main()

