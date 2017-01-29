[![Build Status](https://travis-ci.org/serge-14/gelflib.svg?branch=master)](https://travis-ci.org/serge-14/gelflib)

# gelflib
C++ library for sending Graylog Extended Log Format (GELF) messages using UDP protocol

# Usage

## Initialization

```cpp
    // Initialize GELF library
    gelf::initialize();

    // Configure default host and port number
    gelf::configure( "127.0.0.1", 5000 );

    ...

    // Destroy gelf library
    gelf::destroy();
```

## Simple Message

```cpp
gelf::MessageBuilder message( gelf::Severity::Informational, "Hello world!" );
```

## Complex Message

```cpp
gelf::MessageBuilder message( gelf::Severity::Informational, "Hello world!" )
    .fullMessage( "Full message" )
    .fileName( "main.cpp", 25 )
    .lineNumber( 25 )
    .setHost( "localhost" );
```

## With Additional Fields

```cpp
gelf::MessageBuilder message( gelf::Severity::Informational, "Hello world!" )
    .additionalField( "additional_string", "value" )
    .additionalField( "additional_number", 25 );
```

## Sending Message

```cpp
// Creating message
gelf::MessageBuilder message( gelf::Severity::Informational, "Hello world!" );

// Sending message to configured host
gelf::post( message.build() );
```