#!/bin/bash

# main/sorbet --silence-dev-message -e 'pouts' --no-error-sections 2>&1
main/sorbet --silence-dev-message test/cli/no-error-sections/*.rb --no-error-sections 2>&1
