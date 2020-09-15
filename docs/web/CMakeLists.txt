if(MADNEX_HAVE_PANDOC)
  function(madnex_pandoc_generate_html_page file)
    ADD_CUSTOM_COMMAND(
      OUTPUT    ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
      DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md
      DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/madnex-template.html
      DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/css/main.css
      COMMAND   ${PANDOC}
      ARGS      -f markdown-markdown_in_html_blocks+tex_math_single_backslash
      ARGS      -F pandoc-crossref
      ARGS      --template=${CMAKE_CURRENT_SOURCE_DIR}/madnex-template.html --css=${CMAKE_CURRENT_SOURCE_DIR}/css/main.css
      ARGS      --mathjax
      ARGS      --highlight-style=tango
      ARGS      --email-obfuscation=javascript
      ARGS       --default-image-extension=svg
      ARGS      ${ARGN}
      ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md -o ${file}.html)
    add_custom_target(${file}-html ALL DEPENDS ${file}.html)
    add_dependencies(website ${file}-html)
    if(MADNEX_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
        DESTINATION share/doc/madnex-${MADNEX_SUFFIX}/web
        COMPONENT website)
    else(MADNEX_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
        DESTINATION share/doc/madnex/web
        COMPONENT website)
    endif(MADNEX_APPEND_SUFFIX)
  endfunction(madnex_pandoc_generate_html_page)
endif(MADNEX_HAVE_PANDOC)

add_custom_target(website)
add_dependencies(doc website)

if(MADNEX_HAVE_PANDOC)
  madnex_pandoc_generate_html_page(index)
  madnex_pandoc_generate_html_page(install)
  madnex_pandoc_generate_html_page(contributing)
  madnex_pandoc_generate_html_page(madnex-query)
  madnex_pandoc_generate_html_page(mfront-implementations-storage)
endif(MADNEX_HAVE_PANDOC)

if(MADNEX_APPEND_SUFFIX)
  install(DIRECTORY css img
    DESTINATION share/doc/madnex-${MADNEX_SUFFIX}/web
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
else(MADNEX_APPEND_SUFFIX)
  install(DIRECTORY css img
    DESTINATION share/doc/madnex/web
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
endif(MADNEX_APPEND_SUFFIX)
