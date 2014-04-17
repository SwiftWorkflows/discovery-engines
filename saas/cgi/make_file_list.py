#!/usr/bin/env python

import os
from xml import dom

# List directory
root_directory = os.getenv('root_directory')
listing = os.listdir(root_directory)
print listing

# Create XML
impl = dom.getDOMImplementation()
doc_type = impl.createDocumentType('de-file-list', 'de-id-1', 'de-id-2')
doc = impl.createDocument(dom.EMPTY_NAMESPACE, 'p', doc_type)

# Fill in HTML entries


print doc.documentElement.toxml() 

