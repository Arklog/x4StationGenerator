import re
from typing import Dict

from models.wares import LangData
from xml.dom.minidom import parse


def parse_lang_file(filename: str) -> LangData:
    filecontent = parse(filename)
    retv: LangData = {}

    pages = filecontent.getElementsByTagName("page")
    for page in pages:
        page_id = page.getAttribute("id")
        page_data: Dict[str, str] = {}
        retv[page_id] = page_data

        for item in page.getElementsByTagName("t"):
            item_id = item.getAttribute("id")
            item_value = item.childNodes[0].data

            page_data[item_id] = item_value

    return retv


def get_loc(field: str, lang: LangData) -> str:
    reg = re.compile(r"({\d+,\d+})", flags=re.NOFLAG)
    match = re.search(reg, field)

    while match:
        cmatch = match.group(1)
        cloc = cmatch[1:-1].split(",")
        translation = lang.get(cloc[0], {}).get(cloc[1], "")
        field = field.replace(cmatch, translation)
        match = re.search(reg, field)

    match = re.match(r"\((.+?)\)", field)
    if match:
        return match.group(1)

    return field
