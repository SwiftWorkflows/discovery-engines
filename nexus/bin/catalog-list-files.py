#!/usr/bin/env python

import os

catalog_id = os.getenv("GCAT_DEFAULT_CATALOG_ID")

from globusonline.catalog.client.goauth import get_access_token
from globusonline.catalog.client.dataset_client import DatasetClient
from globusonline.catalog.client.catalog_wrapper import CatalogWrapper
from globusonline.catalog.client.queries import Queries

if __name__ == "__main__":
    wrap = CatalogWrapper(token_file=CatalogWrapper.default_token_file())
    client = wrap.catalogClient
    queries = Queries(client, catalog_id)
    files = queries.list_files(catalog_id)
    for f in files:
        print f, files[f]
        dataset_id = files[f][1]
        member_id = files[f][1]
        print member_id
        annotations = queries.list_member_annotations(catalog_id,
                                                      dataset_id,
                                                      member_id)
        print annotations
