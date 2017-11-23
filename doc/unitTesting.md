# Unit Testing

## Testing Frameworks

### Jest (JavaScript)

[Documentation for Jest](https://facebook.github.io/jest/docs/en/getting-started.html)

Each JavaScript project can be tested individually using the following commands.

```shell
cd <directory of JavaScript project>
npm install
npm run test
```

### Catch 2 (C++)

[Documentation for Catch 2](https://github.com/catchorg/Catch2/blob/master/docs/Readme.md#top)

Each C++ project can be tested individually using the following commands.

```shell
cd <directory of C++ project>
./tests.sh
```

## Travis CI

[Travis CI Dashboard](https://travis-ci.org/USU-Robosub/submarine)

The settings for Travis VMs is located in `.travis.yml`. The `matrix` command is used to spin up two separate VM instances. One VM is used for testing C++ code, and the other VM is used for testing JavaScript code. The VMs will run the same commands as above. However, coverage tracking will be enabled, and the results will be sent to Coveralls.io for tracking.

## Coveralls.io

[Coveralls.io Dashboard](https://coveralls.io/github/USU-Robosub/submarine)

Coveralls.io tracks the amount of code that is currently being tested.
