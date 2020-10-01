## About these GitHub workflows 

### prebuild.yml

This GitHub workflow runs each time a new tag is pushed to the repository.

The workflow builds binaries for each supported operating system and platform. The binaries are uploaded to a new release in the the GitHub repository. These binaries are later downloaded from the GitHub release as part of the process of publishing the npm module. 

The process of publishing the native addon to npm should not be performed until this GitHub workflow runs successfully to completion and the results are verified. 

### publish.yml

This GitHub workflow runs when an authenticated `POST` is made to the GitHub API `/dispatches` endpoint.

The workflow downloads the binaries from the GitHub release created by the prebuild process and publishes the native addon as an npm module. 

This workflow has the following requirements:

- The settings of the GitHub repository where the workflow is running must include an `NPM_TOKEN` GitHub secret containing a valid npm authentication token for the npm account.

- The token specified in the `NPM_TOKEN` GitHub secret must have the **Publish** capability. 

- If Two-Factor Authentication is enabled for the npm account where the module is being published, the "2FA Mode" must be for **Authorization** only and not for **Authorization and Publishing**.

The `publish-github.sh` script in this project's `scripts` directory can be used to issue a correctly formatted `POST` to the GitHub API to trigger this workflow.  

### test.yml

This GitHub workflow runs on each push and pull request. It also runs each Sunday at 04:00 UTC.

The workflow runs routine smoke tests on the code with the current and immediately previous Node.js LTS versions. 

