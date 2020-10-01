## About these scripts

### Publishing

Two scripts are provided for publishing the native addon as an npm module.

#### publish-local.sh

This script publishes the current working directory to the npm registry. 

This script is quick and easy to run, but relies on the user to verify that the contents of the current working directory accurately reflect those of the repository's `master` branch. 

#### publish-github.sh

This script sends a `POST` request to the GitHub API that triggers the `publish.yml` workflow on the `master` branch. Although more involved than the `publish-local.sh` script, this script insures that the contents of the npm module exactly match those of the repository's `master` branch. But perhaps the biggest advantage of this approach is that it creates a permanent and verifiable record of the publishing process on the project's GitHub repository.  

This script has the following requirements:

- The `GITHUB_TOKEN` environment variable on the local machine running the script must contain a GitHub personal access token with `repo` permission for the GitHub repository. 

- The settings of the GitHub repository where the workflow is running must include an `NPM_TOKEN` GitHub secret containing a valid npm authentication token for the npm registry.

- The token specified in the `NPM_TOKEN` GitHub secret must have the **Publish** capability. 

- If Two-Factor Authentication is enabled for the npm account where the module is being published, the "2FA Mode" must be for **Authorization** only and not for **Authorization and Publishing**.
