file (READ "${LEMONS_README}" lemons_readme_content)

set (last_bit_of_intro "whatever floats your git boat.")

string (FIND "${lemons_readme_content}" "${last_bit_of_intro}" chop_idx)
string (LENGTH "${last_bit_of_intro}" last_bit_len)
math (EXPR idx "${chop_idx} + ${last_bit_len}")

string (SUBSTRING "${lemons_readme_content}" ${idx} -1 relevant_text)

file (READ "${MAIN_PAGE_INPUT}" main_page_content)

string (APPEND main_page_content "${relevant_text}")

file (WRITE "${MAIN_PAGE_OUTPUT}" "${main_page_content}")
