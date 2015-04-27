#!/usr/bin/env python

import os

catalog_id = os.getenv("GCAT_DEFAULT_CATALOG_ID")

from globusonline.catalog.client.goauth import get_access_token
from globusonline.catalog.client.dataset_client import DatasetClient

print catalog_id

if __name__ == "__main__":
    goauth_result = get_access_token()
    client = DatasetClient(goauth_result.token, base_url=BASE_URL)
    _, datasets = client.get_datasets()
    for c in catalogs:
        print cat
        print "==== %s ====" % cat["config"]["name"]
        for k, v in cat["config"].iteritems():
            if k == "name":
                continue
            print "%s: %s" % (k, v)
