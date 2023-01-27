# Curriculum Map

![Screenshot](./example.png)

A visualization solution for curriculum and course dependencies of the Hong Kong University of Science and Technology (HKUST) [![GitHub all releases](https://img.shields.io/github/downloads/Zory123/CurriculumMap/total)](https://github.com/zory233/CurriculumMap/releases)

A nice web-based re-implementation can be found
[here](https://github.com/ZHANG-Zhong-HKUST/CurriculumMap-Web).

## Installation

1. Download the zip file
   - Windows
   [![GitHub release (latest by date and asset)](https://img.shields.io/github/downloads/Zory123/CurriculumMap/v1.0/CurriculumMap-win.zip?color=green)](https://github.com/Zory123/CurriculumMap/releases/tag/v1.0)
   - macOS
   [![GitHub release (latest by date and
    asset)](https://img.shields.io/github/downloads/Zory123/CurriculumMap/v1.0/CurriculumMap-mac.dmg?color=green)](https://github.com/Zory123/CurriculumMap/releases/tag/v1.0)
   - Linux
   [![GitHub release (latest by date and
    asset)](https://img.shields.io/github/downloads/Zory123/CurriculumMap/v1.0/CurriculumMap-linux.zip?color=green)](https://github.com/Zory123/CurriculumMap/releases/tag/v1.0)
2. Unzip the zip file.
3. Go to the executable directory and run the program:
   - For Windows: Run `CurriculumMap-win/CurriculumMap.exe`.
   - For Linux: Run `CurriculumMap-linux/CurriculumMap.sh`.
   - For macOS: Run `CurriculumMap-mac/CurriculumMap`
4. (Optional) Update the course information.
   1. First get the newest version of `courses.csv` by running `python
     get-information.py`. `get-information.py` is alongside the executable:
   2. Then replace the `courses.csv` coming with the package (alongside
     `get-information.py`) with the one you just generated.

## Using CurriculumMap

### Flags

- search: search the pre-requisites of the selected course
- reverse search: search the courses that have the selected one as a
  pre-requisite.
- enrolled: tell the program that you have taken this course, so that its
  pre-requisites will no longer be displayed.

You can clear all the flags by clicking on the "Clear Flags" button

### Mouse operations

- To zoom in/out: Pinch on TrackPad; +/- button; mouse wheel
- To move the canvas: Swipe on TrackPad; arrow button; mouse hold&move
- Rightclick course in map to add/delete flags
- Click item above to navigate course(with mouse jumping):

### Legend

By default, only pre-requisites are displayed. You can click on the checkbox on
the right to show/hide pre-requisites, exclusions or co-requisites.

Note that exclusion is directional, i.e. "A excludes B" means you cannot take A
if you have taken B. If A excludes B, there will be a red cross near A on line between A and B

A rectangle represents a set of courses that mutually exclude each others.

### More information

More information about project source structure -> `/project_doc_H26.pdf`

---

insteresting course code: BIEN3320

## For Developers

### Building

Dependencies: Qt Kit v6.1.1 and above (not the creator's version).

1. Clone this repo. Open `./CurriculumMap/CurriculumMap.pro`.
2. Build the project.
3. Put `./get-information/courses.csv` in the same directory as the built
   executable file. The executable file is located in
   `/build-CurriculumMap-.../CurriculumMap.app/Contents/MacOS/` for macOS (you
   need to right click on `CurriculumMap.app` and click "Show Package
   Contents"), or `/build-CurriculumMap-.../` for Windows and Linux.
4. Run the project.

### Deploying

1. Build the project with "Release" mode.
2. Run "Deploy" from the "Build" menu.
2. Zip `./get-information/courses.csv` as well as
   `./get-information/get-information.py` with the generated executable

## Contributor:

XU, Han

WANG, Yucheng

WANG, Yicheng

ZHANG, Yujun
