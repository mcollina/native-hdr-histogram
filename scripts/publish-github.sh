#!/usr/bin/env bash
curl -H "Accept: application/vnd.github.everest-preview+json" \
     -H "Authorization: token ${GITHUB_TOKEN}" \
     --request POST \
     --data '{"ref": "master"}' \
     https://api.github.com/repos/mcollina/native-hdr-histogram/actions/workflows/publish.yml/dispatches
